#include "Enemy.h"
#include "MathUtilityforText.h"
#include <numbers>

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.translation_ = { 2, 2, 20 };
	worldTransform_.rotation_ = { 0, std::numbers::pi_v<float>, 0 };

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	ApproachInitialize();

}

void Enemy::Update()
{

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

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

}

void Enemy::Draw(Camera& camera)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);

	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);


	// 弾を登録する
	bullets_.push_back(newBullet);

}

void Enemy::MoveApproach()
{
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += velocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}


	// 発射タイマーカウントダウン
	fireTimer_--;
	// 指定時間に達した
	if (fireTimer_<=0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}



}

void Enemy::MoveLeave()
{
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += velocityLeave_;
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}
