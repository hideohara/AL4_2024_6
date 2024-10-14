#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;
#include "PlayerBullet.h"


#include <list>


// プレイヤー

class Player
{

public:


    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Player();


    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">モデル</param>
    /// <param name="textureHandle">テクスチャハンドル</param>
    void Initialize(Model* model, uint32_t textureHandle, Model* modelPlayer,Vector3 position);


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
    void Attack();


    // ワールド座標を取得
    Vector3 GetWorldPosition();

    // 衝突を検出したら呼び出されるコールバック関数
    void OnCollision();

    // 弾リストを取得
    const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
    
    /// <summary>
    /// 親となるワールドトランスフォームをセット
    /// </summary>
    /// <param name="parent">親となるワールドトランスフォーム</param>
    void SetParent(const WorldTransform* parent);


private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    Model* modelPlayer_ = nullptr;
    // テクスチャハンドル
    uint32_t textureHandle_ = 0u;

    // キーボード入力
    Input* input_ = nullptr;

    // 弾
    //PlayerBullet* bullet_ = nullptr;

    // 弾
    std::list<PlayerBullet*> bullets_;

};

