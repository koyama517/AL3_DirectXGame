#include "Enemy.h"

void Enemy::Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	
	velocity_ = velocity;

	worldTransform_.translation_ = {0, 3, 30};
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
	worldTransform_.translation_ = Calculation::VectorAdd(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

}

void Enemy::ApproachMove() {

	worldTransform_.translation_ = 
		Calculation::VectorAdd(worldTransform_.translation_,approachVelocity_);

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::LeaveMove() {
	worldTransform_.translation_ =
	    Calculation::VectorAdd(worldTransform_.translation_, leaveVelocity_);
}

void Enemy::Draw(ViewProjection viewPrpjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);
}