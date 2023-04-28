#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include"Calculation.h"
#include"ImGuiManager.h"
#include "WinApp.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {

	Input* input_ = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewprojection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection viewPrpjection);

private:
	//ワールドデータ変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
