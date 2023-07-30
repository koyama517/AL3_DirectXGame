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
	void Initialize(Model* model, Model* bullet ,uint32_t textureHandle, Vector3 position);

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
	void OnCollision() {
		if (HP > 0) {
			HP--;
		}
	};
	void OnCollisionPB() {
		if (nowBulletNum <= MaxNum) {
			nowBulletNum += 1; 
		
		}
	};

	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }

	const float radius_ = 0.5f;
	void SetParent(const WorldTransform* parent);

	void DrawUI();

	void Mouse(ViewProjection viewPrpjection, Matrix4x4 matViewport);

	Vector3 Get3DReticleWorldPosition();

	void MekeBullet(Vector3 pos);


	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	int nowBulletNum;

	bool isInhole;

	int GetHp() { return HP;}

private:
	//ワールドデータ変換
	WorldTransform worldTransform_;

	WorldTransform worldTransForm3DReticle_;

	//モデル
	Model* model_ = nullptr;

	Model* bulletModel_ = nullptr;

	Sprite* sprite2DReticle_ = nullptr;

	Sprite* bulletUI_ = nullptr;
	Sprite* magUI = nullptr;
	Sprite* magNumten_ = nullptr;
	Sprite* magNum_ = nullptr;

	Sprite* numsUI_ = nullptr;
	Sprite* numsUIten_ = nullptr;

	Sprite* hpUI_ = nullptr;
	Sprite* hpNum_ = nullptr;

	Sprite* RBUI_ = nullptr;
	Sprite* LBUI_ = nullptr;

	Sprite* lineUI_ = nullptr;
	Sprite* pUI_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	uint32_t reticleHandle_ = 0u;

	uint32_t bulletUIHandle_ = 0;

	uint32_t magnetUIHandle_ = 0;
	uint32_t useMagnetUIHandle_ = 0;

	uint32_t numsHandle_[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	uint32_t hpHandle_ = 0;

	uint32_t RBHandle_ = 0;
	uint32_t LBHandle_ = 0;

	uint32_t lineHandle_ = 0;
	uint32_t pUIHandle_ = 0;

	//PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	static const uint32_t MaxNum = 50;

	static const uint32_t startNum = 10;

	const int ShotInterval = 30;

	int shotTimer = 0;

	bool isMagTime;

	bool canMag;
	const int magInterval = 300;

	int magTimer = 0;

	float uiPos = 340;

	int HP = 3;

};
