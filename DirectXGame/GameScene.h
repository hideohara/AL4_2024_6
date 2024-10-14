#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();


	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleEnemy_ = 0;

	// プレイヤー3Dモデル
	Model* model_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// カメラ
	Camera camera_;

	// 各クラス
	Player* player_ = nullptr;
	//Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵
	std::list<Enemy*> enemies_;


	//  敵発生コマンド
	std::stringstream enemyPopCommands;


	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();


	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void PopEnemy(Vector3 position);

	bool enemyFlag_ = false;
	int enemyTimer_ = 0;
};
