#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {0.9f, 0.9f, 0.9f};
	velocity_ = velocity;
}

void EnemyBullet::Update(bool isHole, Vector3 playerPos) {

	if (isHole) {
		Vector3 toPlayer = Calculation::VectorSubtraction(playerPos, worldTransform_.translation_);
		toPlayer = Calculation::Normalize(toPlayer);
		velocity_ = Calculation::Normalize(velocity_);

		velocity_ = Calculation::Slerp(velocity_, toPlayer, 0.06f);
	}
	
	worldTransform_.translation_ =
			Calculation::VectorAdd(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() { return worldTransform_.translation_; }
