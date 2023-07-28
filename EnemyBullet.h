#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"Player.h"
#include <assert.h>

/// <summary>
/// 自キャラの弾
/// </summary>
class EnemyBullet {
public:
	///< summary>
	/// 初期化
	///</summary>
	///< param name="model">モデル</param>
	/// <param name="position">初期化</param>
	void Initialize(Model* model, const Vector3& position, const Vector3 velocity);

	///< summary>
	/// 更新
	///</summary>
	void Update(bool isHole, Vector3 playerPos);

	///< summary>
	/// 初期化
	///</summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool isDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition();

	const float radius_ = 0.5f;

	void SetPlayer(Player* player) { player_ = player; }

private:

	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	Player* player_ = nullptr;

	// 寿命
	static const int32_t kLifeTime = 60 * 2;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};