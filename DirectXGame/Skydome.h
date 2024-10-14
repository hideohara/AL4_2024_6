#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

class Skydome
{

public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">モデル</param>
    void Initialize(Model* model);


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
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;

};

