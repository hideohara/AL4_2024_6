#include "Player.h"
#include "MathUtilityForText.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Update()
{

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_ += move;

	// 押した方向で回転
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= 0.05f;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += 0.05f;
	}



	// キーボード入力による移動処理

	// 移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	// 行列を更新
	worldTransform_.UpdateMatrix();


	// キャラクターの座標を画面表示する処理
	ImGui::Begin("DATA");
	ImGui::InputFloat3("Player", &worldTransform_.translation_.x);
	ImGui::End();

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	if (bullet_) {
		bullet_->Update();
	}

}

void Player::Draw(Camera& camera)
{

	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);


	// 弾描画
	if (bullet_) {
		bullet_->Draw(camera);
	}

}

// 攻撃
void Player::Attack()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullet_ = newBullet;
	}

}


