#include "GameScene.h"
//#include "TextureManager.h"
#include <cassert>
#include "MathUtilityforText.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;

	delete player_;
	//delete enemy_;
	delete skydome_;
	delete railCamera_;

	delete debugCamera_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandleEnemy_ = TextureManager::Load("enemy.png");

	// 3Dモデルの生成
	model_ = Model::Create(); 

	// カメラ初期化
	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	//player_->Initialize(model_,textureHandle_);

	Vector3 playerPosition(0, 0, 50.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);

	//// 敵キャラの生成
	//Enemy* enemy = new Enemy();
	//// 敵キャラの初期化
	//enemy->Initialize(model_, textureHandleEnemy_);
	//// 敵キャラに自キャラのアドレスを渡す
	//enemy->SetPlayer(player_);
	//// 敵キャラにゲームシーンを渡す
	//enemy->SetGameScene(this);
	//// 敵を登録する
	//enemies_.push_back(enemy);
	

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 960);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドームの生成
	skydome_ = new Skydome();
	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_);


	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));


	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// クラスの更新
	player_->Update();
	//enemy_->Update();
	skydome_->Update();

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView; //デバッグカメラのビュー行列;
		camera_.matProjection = debugCamera_->GetCamera().matProjection; // デバッグカメラのプロジェクション行列;

		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	}
	else {
		railCamera_->Update();
		camera_.matView = railCamera_->GetCamera().matView; //デバッグカメラのビュー行列;
		camera_.matProjection = railCamera_->GetCamera().matProjection; // デバッグカメラのプロジェクション行列;
		// ビュープロジェクション行列の更新と転送
		//camera_.UpdateMatrix();
		camera_.TransferMatrix();
	}

	// 衝突判定と応答
	CheckAllCollisions();

	// 敵更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}


	// 敵弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
	if (bullet->IsDead()) {
		delete bullet;
		return true;
	}
	return false;
	});



	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
		});


	UpdateEnemyPopCommands();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>


	// クラスの描画
	player_->Draw(camera_);
	//enemy_->Draw(camera_);
	skydome_->Draw(camera_);

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(camera_);
	}
	// 敵描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

// 衝突判定と応答
void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標の差分ベクトル
		Vector3 subtract = posB - posA;
		// 座標AとBの距離を求める
		float distance = Length(subtract);

		// 球と球の交差判定
		if (distance < 1.5f + 1.5f) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}


#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラと自弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 座標の差分ベクトル
			Vector3 subtract = posB - posA;
			// 座標AとBの距離を求める
			float distance = Length(subtract);

			// 球と球の交差判定
			if (distance < 1.5f + 1.5f) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自弾全ての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();

		// 敵弾全ての当たり判定
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標の差分ベクトル
			Vector3 subtract = posB - posA;
			// 座標AとBの距離を求める
			float distance = Length(subtract);

			// 球と球の交差判定
			if (distance < 1.5f + 1.5f) {
				// 自キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	

#pragma endregion

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}


void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}


void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (enemyFlag_) {
		enemyTimer_--;
		if (enemyTimer_ <= 0) {
			// 待機完了
			enemyFlag_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');


		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			PopEnemy(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			enemyFlag_ = true;
			enemyTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}




	}
}

void GameScene::PopEnemy(Vector3 position)
{
	// 敵キャラの生成
	Enemy* enemy = new Enemy();
	// 敵キャラの初期化
	enemy->Initialize(model_, textureHandleEnemy_,position);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);
	// 敵を登録する
	enemies_.push_back(enemy);
}







