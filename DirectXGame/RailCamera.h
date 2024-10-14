#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

class RailCamera
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Vector3 position, Vector3 rotation);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// ビュープロジェクションを取得
    /// </summary>
    /// <returns>ビュープロジェクション</returns>
    const Camera& GetCamera() { return camera_; }
    // ワールド行列を取得
    const WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Camera camera_;

};

