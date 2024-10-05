#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();


}

void Player::Draw(Camera& camera)
{

	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);


}


