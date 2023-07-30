#include "Enemy.h"
#include "GameScene.h"
#include "Player.h"
void Enemy::Initiarize(Model* model, const Vector3 pos, const Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("ene.png");

	worldTransform_.Initialize();

	velocity_ = velocity;

	Vector3 tmp = {0, 5, 0};

	worldTransform_.translation_ = pos;
	spownPos = worldTransform_.translation_;
	Enemy::InitiarizeApproach();

	// isDead_ = false;
}

Enemy::~Enemy() {}

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
}

void Enemy::ApproachMove() {
	assert(gameScene_);

	worldTransform_.translation_ =
	Calculation::VectorAdd(worldTransform_.translation_, velocity_);

	if (worldTransform_.translation_.x <= spownPos.x - 5 ||
	    worldTransform_.translation_.x >= spownPos.x + 5) {
		velocity_.x *= -1;
	}

	if (worldTransform_.translation_.y <= spownPos.y - 5 ||
	    worldTransform_.translation_.y >= spownPos.y + 5) {
		velocity_.y *= -1;
	}

	shotTimer_--;
	if (shotTimer_ <= 0) {

		Fire();
		shotTimer_ = kFireInterval;
	}
}

void Enemy::LeaveMove() {

	worldTransform_.translation_ =
	    Calculation::VectorAdd(worldTransform_.translation_, leaveVelocity_);
}

void Enemy::Draw(ViewProjection viewPrpjection) {
	if (isDead_ == false) {
		// 3Dモデルの描画
		model_->Draw(worldTransform_, viewPrpjection, textureHandle_);
	}
}

void Enemy::InitiarizeApproach() { shotTimer_ = kFireInterval; }

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos = {
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z,
	};

	return worldPos;
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度

	const float kBulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetWorldPosition();

	Vector3 enemyPos = GetWorldPosition();
	Vector3 e2p = Calculation::VectorSubtraction(playerPos, enemyPos);

	Vector3 normal = Calculation::Normalize(e2p);

	Vector3 velocity = Calculation::Multiply(kBulletSpeed, normal);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, enemyPos, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::Spawn(Vector3 pos) {
	isDead_ = false;
	worldTransform_.translation_ = pos;
}
