#include "GameScene.h"
//#include "TextureManager.h"
#include <cassert>
#include "MathUtilityforText.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;

	delete player_;
	delete enemy_;
	delete skydome_;
	delete railCamera_;

	delete debugCamera_;

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

	Vector3 playerPosition(0, 0, 15.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);

	// 敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, textureHandleEnemy_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);


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
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// クラスの更新
	player_->Update();
	enemy_->Update();
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
	enemy_->Draw(camera_);
	skydome_->Draw(camera_);


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
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
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
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自弾全ての当たり判定
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
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自弾全ての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();

		// 敵弾全ての当たり判定
		for (EnemyBullet* enemyBullet : enemyBullets) {
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
