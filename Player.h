#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include"Calculation.h"
#include"ImGuiManager.h"
#include "WinApp.h"
#include"PlayerBullet.h"
#include<list>
#include"Sprite.h"


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
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection viewPrpjection);

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
	Vector3 GetRotate() {return worldTransform_.rotation_; }
	void OnCollision(){}
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }

	const float radius_ = 0.5f;
	void SetParent(const WorldTransform* parent);

	void DrawUI();

private:
	//ワールドデータ変換
	WorldTransform worldTransform_;

	WorldTransform worldTransForm3DReticle_;

	//モデル
	Model* model_ = nullptr;

	Sprite* sprite2DReticle_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	uint32_t reticleHandle_ = 0u;

	//PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


};
