#include "GameScene.h"
#include "TextureManager.h"
#include"AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//3Dモデルデータの解放
	delete playerModel_;

	//自キャラの解放
	delete player_;

	delete debugCamera_;

	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//テクスチャを読み込む
	playerTextureHandle_ = TextureManager::Load("sample.png");

	//3Dモデルデータの生成
	playerModel_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewPlojection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(playerModel_,playerTextureHandle_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewPlojection_);

	// テクスチャを読み込む
	enemyTextureHandle_ = TextureManager::Load("black1x1.png");

	// 3Dモデルデータの生成
	enemyModel_ = Model::Create();

	const Vector3 enemyvelocity = {0, 0, -0.5f};

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initiarize(enemyModel_, enemyTextureHandle_,enemyvelocity);

	enemy_->SetPlayer(player_);
}

void GameScene::Update() {

	player_->Update();
	debugCamera_->Update();
	enemy_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_E)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();

		viewPlojection_.matView = debugCamera_->GetViewProjection().matView;
		viewPlojection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewPlojection_.TransferMatrix();
	} else {
		//
		viewPlojection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

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

	player_->Draw(viewPlojection_);
	enemy_->Draw(viewPlojection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
