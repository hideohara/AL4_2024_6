#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;
#include "EnemyBullet.h"

#include <list>

// 自機クラスの前方宣言
class Player;

class Enemy
{
public:

    ~Enemy();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">モデル</param>
    /// <param name="textureHandle">テクスチャハンドル</param>
    void Initialize(Model* model, uint32_t textureHandle);




    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
    void Draw(Camera& camera);

    /// <summary>
    /// 攻撃
    /// </summary>
    void Fire();


    void SetPlayer(Player* player) { player_ = player; }

    // ワールド座標を取得
    Vector3 GetWorldPosition();


    // 衝突を検出したら呼び出されるコールバック関数
    void OnCollision();

    // 弾リストを取得
    const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:

    // 行動フェーズ
    enum class Phase {
        Approach, // 接近する
        Leave,    // 離脱する
    };


    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    // テクスチャハンドル
    uint32_t textureHandle_ = 0u;
    // 速度
    Vector3 velocity_ = { 0, 0, -0.03f };
    Vector3 velocityLeave_ = { -0.3f, 0.3f, -0.1f };
    // フェーズ
    Phase phase_ = Phase::Approach;

    // 弾
    std::list<EnemyBullet*> bullets_;

    void MoveApproach();
    void MoveLeave();

    // 発射間隔
    static const int kFireInterval = 60;
    // 発射タイマー
    int32_t fireTimer_ = 0;
    // 接近フェMoveApproachーズ初期化
    void ApproachInitialize();



    // 自キャラ
    Player* player_ = nullptr;

};

