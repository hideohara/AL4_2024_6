#include "Skydome.h"


void Skydome::Initialize(Model* model)
{
#ifdef _DEBUG
	// NULLポインタチェック
	assert(model);
#endif

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(Camera& camera)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera);
}
