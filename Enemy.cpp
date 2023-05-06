#include "Enemy.h"

void Enemy::Initiarize(Model* model, uint32_t textureHandle, const Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	
	velocity_ = velocity;
}

void Enemy::Update() {

	worldTransform_.translation_ = Calculation::VectorAdd(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection viewPrpjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);
}