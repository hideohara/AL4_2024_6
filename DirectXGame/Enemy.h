#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

class Enemy
{
public:
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
    Vector3 velocity_ = { 0, 0, -0.2f };
    Vector3 velocityLeave_ = { -0.3f, 0.3f, -0.2f };
    // フェーズ
    Phase phase_ = Phase::Approach;

    void MoveApproach();
    void MoveLeave();
};

