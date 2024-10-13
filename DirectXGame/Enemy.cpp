#include "Enemy.h"
#include "MathUtilityforText.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.translation_ = { 0, 2, 20 };

	// ワールド変換の初期化
	worldTransform_.Initialize();

}

void Enemy::Update()
{
	// 移動
	//worldTransform_.translation_ += velocity_;

	switch (phase_) {
	case Phase::Approach:
	default:
		MoveApproach();
		break;
	case Phase::Leave:
		MoveLeave();
		break;
	}



	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);


}

void Enemy::MoveApproach()
{
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += velocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::MoveLeave()
{
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += velocityLeave_;
}
