#include "RailCamera.h"
#include "MathUtilityForText.h"

void RailCamera::Initialize(Vector3 position, Vector3 rotation)
{
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	//引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = rotation;

	worldTransform_.Initialize();

	// ビュープロジェクションの初期化
	camera_.farZ = 2000.0f;
	camera_.Initialize();


}

void RailCamera::Update()
{
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	// ワールド行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);

	// ビュープロジェクションを転送
	// 教材に書いていない
	camera_.TransferMatrix();

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	//スライダーでカメラのtranslationを表示
	//	スライダーでカメラのrotationを表示
	ImGui::End();
}
