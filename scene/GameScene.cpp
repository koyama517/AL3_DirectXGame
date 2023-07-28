#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include<fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 3Dモデルデータの解放
	delete playerModel_;

	// 自キャラの解放
	delete player_;

	delete debugCamera_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete skydome_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	TextureManager::Load("reticle.png");
	// テクスチャを読み込む
	playerTextureHandle_ = TextureManager::Load("sample.png");

	// 3Dモデルデータの生成
	playerModel_ = Model::CreateFromOBJ("player",true);
	enemyModel_ = Model::Create();
	bulletModel_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewPlojection_.Initialize();

	Vector3 playerPosition(0, 0, 40.0f);

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(playerModel_, bulletModel_, playerTextureHandle_, playerPosition);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewPlojection_);

	// テクスチャを読み込む
	enemyTextureHandle_ = TextureManager::Load("black1x1.png");

	bulletUIHandle_ = TextureManager::Load("bullets.png");

	magnetUIHandle_ = TextureManager::Load("magnet.png");
	
	useMagnetUIHandle_ = TextureManager::Load("useMagnet.png");

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	// カメラ生成
	railCamera_ = new RailCamera();
	// カメラの初期化
	railCamera_->Initialize(worldTransform_.translation_, worldTransform_.rotation_);

	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();


}

void GameScene::Update() {
	CheckAllCollision();

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
		railCamera_->Update();
		// viewPlojection_.UpdateMatrix();
		viewPlojection_.matView = railCamera_->GetViewProjection().matView;
		viewPlojection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewPlojection_.TransferMatrix();
	}

	player_->Update(viewPlojection_);
	UpdateEnemyPopCommands();
	// debugCamera_->Update();
	for (Enemy* enemy : enemies_) {
		if (enemy->GetIsDead() == false) {
			enemy->Update();
		}
	}
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	// 敵の弾
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {

		bullet->Update(player_->isInhole,player_->GetWorldPosition());
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
	skydome_->Draw(viewPlojection_);
	player_->Draw(viewPlojection_);
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewPlojection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewPlojection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {

	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	const std::list<EnemyBullet*>& enemyBullets = GetBullet();

#pragma region 自キャラと自弾の当たり判定
	posA = player_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		posB = bullet->GetWorldPosition();

		Vector3 length = Calculation::VectorSubtraction(posB, posA);

		if (powf(length.x, 2) + powf(length.y, 2) + powf(length.z, 2) <=
		    powf((player_->radius_ + bullet->radius_), 2)) {
			if (bullet->canHole) {

				player_->OnCollisionPB();
			
				bullet->OnCollision();
			
			}
		}
	}
#pragma endregion
#pragma region 自キャラと敵弾の当たり判定

	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {

		posB = bullet->GetWorldPosition();

		Vector3 length = Calculation::VectorSubtraction(posB, posA);

		if (powf(length.x, 2) + powf(length.y, 2) + powf(length.z, 2) <=
		    powf((player_->radius_ + bullet->radius_), 2)) {

			player_->OnCollision();

			bullet->OnCollision();
			isHit++;
		}
	}

#pragma endregion
#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemies_) {

		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {

			posB = bullet->GetWorldPosition();

			Vector3 length = Calculation::VectorSubtraction(posB, posA);

			if (powf(length.x, 2) + powf(length.y, 2) + powf(length.z, 2) <=
			    powf((enemy->radius_ + bullet->radius_), 2)) {

				enemy->OnCollision();

				bullet->OnCollision();

				enemy->SetIsDead(true);
			}
		}
	}

#pragma endregion
/* #pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* eBullet : enemyBullets) {
		posA = eBullet->GetWorldPosition();
		for (PlayerBullet* pBullet : playerBullets) {

			posB = pBullet->GetWorldPosition();

			Vector3 length = Calculation::VectorSubtraction(posB, posA);

			if (powf(length.x, 2) + powf(length.y, 2) + powf(length.z, 2) <=
			    powf((eBullet->radius_ + pBullet->radius_), 2)) {

				pBullet->OnCollision();

				eBullet->OnCollision();
			}
		}
	}

#pragma endregion*/
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() { 
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() { 
	
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			isWait = false;
		}
		return;
	}
	
	std::string line;

	while (std::getline(enemyPopCommands, line)) {
		std::istringstream line_stram(line);

		std::string word;
		std::getline(line_stram, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {

			std::getline(line_stram, word, ',');
			float x = (float)std::atof(word.c_str());
		
			std::getline(line_stram, word, ',');
			float y = (float)std::atof(word.c_str());

			std::getline(line_stram, word, ',');
			float z = (float)std::atof(word.c_str());

			Spawn(Vector3(x,y,z));
		}

		else if (word.find("WAIT") == 0) {
			std::getline(line_stram, word, ',');
			int32_t waitTime = atoi(word.c_str());

			isWait = true;
			waitTimer = waitTime;
			break;
		}
	}
}
void GameScene::Spawn(Vector3 pos) {
	// 敵の生成
	const Vector3 enemyvelocity = {0, 0, -0.5f};
	// enemy_ = new Enemy();
	Enemy* newEnemy = new Enemy();
	// 敵の初期化
	newEnemy->Initiarize(enemyModel_, pos, enemyvelocity);
	enemies_.push_back(newEnemy);
	// enemy_->Initiarize(enemyModel_, enemyTextureHandle_, enemyvelocity);
	// 敵キャラにゲームシーンを渡す
	for (Enemy* enemy : enemies_) {
		enemy->SetGameScene(this);
		enemy->SetPlayer(player_);
	}
}
