#include "Skydome.h"
#include <cassert>
void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;


	worldTransform_.Initialize();
}

void Skydome::Update() {

	worldTransform_.rotation_.y += 0.0001f;

	worldTransform_.UpdateMatrix();

}


void Skydome::Draw(ViewProjection viewPrpjection) { 

	model_->Draw(worldTransform_, viewPrpjection); }
