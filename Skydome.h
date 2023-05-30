#pragma once
#include"Model.h"
#include"WorldTransform.h"
class Skydome {

	public:
	/// <summary>
	/// 初期化
	/// </summary>
	    void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewPrpjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};
