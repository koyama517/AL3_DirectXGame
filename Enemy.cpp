#include "Enemy.h"
#include"Player.h"
void Enemy::Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	
	velocity_ = velocity;

	worldTransform_.translation_ = {10, 0, 50};

	Enemy::InitiarizeApproach();

}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Update() {
	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		ApproachMove();
		break;
	case Enemy::Phase::Leave:
		LeaveMove();
		break;
	}
	worldTransform_.UpdateMatrix();

	for (EnemyBullet* bullet : bullets_) {

		bullet->Update();
	}

}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度

	const float kBulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = this->GetWorldPosition();

	Vector3 e2p = Calculation::VectorSubtraction(playerPos,enemyPos);

	Vector3 normal = Calculation::Normalize(e2p);

	Vector3 velocity = Calculation::Multiply(kBulletSpeed, normal);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}


void Enemy::ApproachMove() {

	worldTransform_.translation_ = 
		Calculation::VectorAdd(worldTransform_.translation_,approachVelocity_);

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	shotTimer_--;
	if (shotTimer_ <= 0) {

		Enemy::Fire();

		shotTimer_ = kFireInterval;

	}


}

void Enemy::LeaveMove() {
	worldTransform_.translation_ =
	    Calculation::VectorAdd(worldTransform_.translation_, leaveVelocity_);
}

void Enemy::Draw(ViewProjection viewPrpjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewPrpjection);
	}
}

void Enemy::InitiarizeApproach() { 
	shotTimer_ = kFireInterval; 
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos = {
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z,
	};

	return worldPos;
}