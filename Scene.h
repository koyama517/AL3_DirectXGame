#pragma once
#include"GameScene.h"
class Scene {
public:
	void Initialize();

	void Draw(int s);

public:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t title = 0;
	uint32_t clear = 0;
	uint32_t over = 0;
	Sprite* spriteScene = nullptr;
};
