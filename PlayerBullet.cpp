#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity, bool isStop) { 
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("green.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

	worldTransform_.scale_ = Vector3(0.8f,0.8f,0.8f);

	canHit = isStop;
}

void PlayerBullet::Update(bool isHole, const Vector3& playerPos) {
	if (isStop_ == false && !isHole) {
		worldTransform_.translation_ =
		    Calculation::VectorAdd(worldTransform_.translation_, velocity_);
		worldTransform_.UpdateMatrix();
	} 
	else if (isHole && canHole) {
		isStop_ = false;
		Vector3 toPlayer = Calculation::VectorSubtraction(playerPos, worldTransform_.translation_);
		toPlayer = Calculation::Normalize(toPlayer);
		velocity_ = Calculation::Normalize(velocity_);

		velocity_ = Calculation::Slerp(velocity_, toPlayer, 1);

		worldTransform_.translation_ =
		    Calculation::VectorAdd(worldTransform_.translation_, velocity_);

		worldTransform_.UpdateMatrix();
	} 
	else if (isStop_ && !isHole){
		velocity_ = Vector3(0, 0, -0.2f);
		worldTransform_.translation_ =
		    Calculation::VectorAdd(worldTransform_.translation_, velocity_);
		worldTransform_.UpdateMatrix();
	}



	if (isStop_) {
		canHole = true;
		canHit = false;
	}


	if (--deathTimer_ <= 0 && canHole == false) {
		isStop_ = true;
	}
	
	if (worldTransform_.translation_.z < playerPos.z - 30) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	
	model_->Draw(worldTransform_,viewProjection,textureHandle_);

}

void PlayerBullet::OnCollision() {
	
	isDead_ = true;

}

Vector3 PlayerBullet::GetWorldPosition() {

	return worldTransform_.translation_;

}