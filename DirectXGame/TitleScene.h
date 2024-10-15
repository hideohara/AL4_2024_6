#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class TitleScene
{
public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	// 終了フラグ
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;

	// スプライト
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;


};

