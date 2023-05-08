#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<assert.h>
/// <summary>
/// 敵
/// </summary>
class Enemy {

	//行動フェーズ
	enum class Phase {
		Approach,
		Leave,
	};

public:

	void Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity);

	void Update();

	void Draw(ViewProjection viewPrpjection);

	void ApproachMove();

	void LeaveMove();

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	Vector3 approachVelocity_ = {0, 0, 0.1f};

	Vector3 leaveVelocity_ = {0.1f, 0.1f, 0.1f};

	//フェーズ
	Phase phase_ = Phase::Approach;


};
