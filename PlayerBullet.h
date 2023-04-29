#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<assert.h>
    /// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:
	///<summary>
	///初期化
	///</summary>
	///<param name="model">モデル</param>
	/// <param name="position">初期化</param>
	void Initialize(Model* model, const Vector3& position);

	///<summary>
	///更新
	///</summary>
	void Update();

		///< summary>
	/// 初期化
	///</summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
