﻿#include "Player.h"
#include <cassert>
void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	model_  = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

	

}

void Player::Update() {

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
	worldTransform_.matWorld_ = Calculation::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	ImGui::Begin("a");
	ImGui::SliderFloat3("Player", pos, 0.0f, 1280);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];
	ImGui::End();
}

void Player::Draw(ViewProjection viewPrpjection) {

	//3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);

}
