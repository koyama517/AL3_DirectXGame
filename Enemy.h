#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include<assert.h>

//自機クラスの前方宣言
class Player;

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

	static const int kFireInterval = 60;

	void Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity);

	~Enemy();

	void Update();

	void Draw(ViewProjection viewPrpjection);

	void ApproachMove();

	void LeaveMove();

	void InitiarizeApproach();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	Vector3 approachVelocity_ = {0.0f, 0.0f, 0.0f};

	Vector3 leaveVelocity_ = {0.1f, 0.1f, 0.1f};

	//フェーズ
	Phase phase_ = Phase::Approach;

	std::list<EnemyBullet*> bullets_;

	void Fire();

	int32_t shotTimer_ = 0;

	Player* player_ = nullptr;
};
