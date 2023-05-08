#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include"Calculation.h"
#include"ImGuiManager.h"
#include "WinApp.h"
#include"PlayerBullet.h"
#include<list>


/// <summary>
/// 自キャラ
/// </summary>
class Player {

	Input* input_ = nullptr;

public:

	// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewprojection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection viewPrpjection);

	Vector3 GetWorldPosition();

private:
	//ワールドデータ変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


};
