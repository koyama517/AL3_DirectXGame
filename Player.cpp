﻿#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	//NULLポインタチェック
	assert(model);

	model_  = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	input_ = Input::GetInstance();

}

Player::~Player() {


	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Update() {

	//デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//行列を指定バッファに転送
	worldTransform_.TransferMatrix();
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//デバッグ用
	float pos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z}; 
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;	
	}

	//移動限界
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_ = Calculation::VectorAdd(worldTransform_.translation_,move);
	
	pos[0] = worldTransform_.translation_.x;
	pos[1] = worldTransform_.translation_.y;
	pos[2] = worldTransform_.translation_.z;
	worldTransform_.UpdateMatrix();

	ImGui::Begin("a");
	ImGui::SliderFloat3("Player", pos, 0.0f, 1280);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];
	ImGui::End();

	// 旋回
	Rotate();

	// 攻撃処理
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Rotate() { 
	//回転の速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変換
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if(input_->PushKey(DIK_D)){
		worldTransform_.rotation_.y += kRotSpeed;

	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度

		const float kBulletSpeed = 1.0f;

		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Calculation::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		
		bullets_.push_back(newBullet);
	}

}

void Player::Draw(ViewProjection viewPrpjection) {

	//3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewPrpjection);
	}

}

Vector3 Player::GetWorldPosition() { 

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent; 
}