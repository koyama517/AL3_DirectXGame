#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include<assert.h>

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言
class GameScene;

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

	void Initiarize(Model* model, Vector3 pos, const Vector3 velocity);

	~Enemy();

	void Update();

	void Draw(ViewProjection viewPrpjection);

	void ApproachMove();

	void LeaveMove();

	void InitiarizeApproach();

	void SetPlayer(Player* player) { player_ = player; }
	
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetIsDead(bool dead) { isDead_ = dead;}

	Vector3 GetWorldPosition();

	void Spawn(Vector3 pos);

	void OnCollision() {}
	void Fire();
	//const std::list<EnemyBullet*>& GetBullet() { return bullets_; }

	const float radius_ = 3.0f;

	bool GetIsDead() { return isDead_; }

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	Vector3 approachVelocity_ = {0.0f, 0.0f, 0.01f};

	Vector3 leaveVelocity_ = {0.1f, 0.1f, 0.1f};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//std::list<EnemyBullet*> bullets_;

	int32_t shotTimer_ = 0;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	bool isDead_;
};
