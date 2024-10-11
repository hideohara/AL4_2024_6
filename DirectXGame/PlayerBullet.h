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
    void Initialize(Model* model, const Vector3& position);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();


    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection">ビュープロジェクション</param>
    void Draw(const Camera& camera);

private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    // テクスチャハンドル
    uint32_t textureHandle_ = 0u;

};

