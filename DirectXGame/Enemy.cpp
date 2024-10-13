#include "Enemy.h"
#include "MathUtilityforText.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.translation_ = { 0, 2, 10 };

	// ワールド変換の初期化
	worldTransform_.Initialize();

}

void Enemy::Update()
{
	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);


}
