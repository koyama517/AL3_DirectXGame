#include "Skydome.h"
#include <cassert>
void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;


	worldTransform_.Initialize();
}

void Skydome::Update() {

}


void Skydome::Draw(ViewProjection viewPrpjection) { 

	model_->Draw(worldTransform_, viewPrpjection); }
