#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 position, Vector3 rad) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rad;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	worldTransform_.translation_.z += 0.1f; 
	//worldTransform_.rotation_.y += 0.001f;
	worldTransform_.matWorld_ =
	    Calculation::MakeAffineMatrix({1,1,1}, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Calculation::Inverse(worldTransform_.matWorld_);

	//ImGui::Begin("camera");
	//ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -10.0f, 10.0f);
	//ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -100.0f, 100.0f);
	//ImGui::End();
	viewProjection_.TransferMatrix();
}