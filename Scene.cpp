#include "Scene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include<fstream>
void Scene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	title = TextureManager::Load("/title.png");
	clear = TextureManager::Load("/clear.png");
	over = TextureManager::Load("/over.png");
	spriteScene = Sprite::Create(title, Vector2(0, 0), Vector4(1, 1, 1, 1), Vector2(0, 0));
}

void Scene::Draw(int s) {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (s == 0) {
		spriteScene->SetTextureHandle(title);
		spriteScene->Draw();
	} else if (s == 2) {
		spriteScene->SetTextureHandle(clear);

		spriteScene->Draw();
	} else if (s == 3) {
		spriteScene->SetTextureHandle(over);

		spriteScene->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}