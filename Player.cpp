#include "Player.h"
#include <cassert>
void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	model_  = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

}

void Player::Update() {

	//行列を指定バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection viewPrpjection) {

	//3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);

}
