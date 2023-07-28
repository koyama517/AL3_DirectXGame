#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include"DebugCamera.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	//3Dモデル
	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* reticleModel_ = nullptr;
	Model* bulletModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewPlojection_;

	//自キャラ
	Player* player_ = nullptr;

	//デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	
	//天球
	Skydome* skydome_ = nullptr;
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollision();
	/// <summary>
	/// 敵弾を追加する。
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();

	void Spawn(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	std::list<EnemyBullet*> enemyBullets_;
	const std::list<EnemyBullet*>& GetBullet() { return enemyBullets_; }

	//敵
	std::list<Enemy*> enemies_;
	const std::list<Enemy*>& GetEnemy() { return enemies_; }

	std::stringstream enemyPopCommands;

	bool isWait = false; 
	int waitTimer;

	int isHit;

	uint32_t bulletUIHandle_ = 0;
	uint32_t magnetUIHandle_ = 0;
	uint32_t useMagnetUIHandle_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
