#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<assert.h>
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:

	void Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity);

	void Update();

	void Draw(ViewProjection viewPrpjection);

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

};
