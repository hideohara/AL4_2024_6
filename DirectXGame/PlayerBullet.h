#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

// プレイヤー弾
class PlayerBullet
{

public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">モデル</param>
    /// <param name="position">初期座標</param>
    /// <param name="velocity">速度</param>
    void Initialize(Model* model, const Vector3& position, const Vector3& velocity);



    /// <summary>
    /// 更新
    /// </summary>
    void Update();


    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection">ビュープロジェクション</param>
    void Draw(const Camera& camera);

    bool IsDead() const { return isDead_; }

    // 衝突を検出したら呼び出されるコールバック関数
    void OnCollision();

    // ワールド座標を取得
    Vector3 GetWorldPosition();

private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    // テクスチャハンドル
    uint32_t textureHandle_ = 0u;

    // 速度
    Vector3 velocity_;

    // 寿命<frm>
    static const int32_t kLifeTime = 60 * 5;


    // デスタイマー
    int32_t deathTimer_ = kLifeTime;
    // デスフラグ
    bool isDead_ = false;



};

