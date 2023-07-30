#include "Player.h"
#include "ViewProjection.h"
#include <cassert>

void Player::Initialize(Model* model, Model* bullet, uint32_t textureHandle, Vector3 position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	bulletModel_ = bullet;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	input_ = Input::GetInstance();
	worldTransForm3DReticle_.Initialize();

	reticleHandle_ = TextureManager::Load("reticle.png");

	bulletUIHandle_ = TextureManager::Load("UI/bullets.png");

	magnetUIHandle_ = TextureManager::Load("UI/magnet.png");

	useMagnetUIHandle_ = TextureManager::Load("UI/useMag.png");

	hpHandle_ = TextureManager::Load("UI/hearts.png");

	numsHandle_[0] = TextureManager::Load("numbers/0.png");
	numsHandle_[1] = TextureManager::Load("numbers/1.png");
	numsHandle_[2] = TextureManager::Load("numbers/2.png");
	numsHandle_[3] = TextureManager::Load("numbers/3.png");
	numsHandle_[4] = TextureManager::Load("numbers/4.png");
	numsHandle_[5] = TextureManager::Load("numbers/5.png");
	numsHandle_[6] = TextureManager::Load("numbers/6.png");
	numsHandle_[7] = TextureManager::Load("numbers/7.png");
	numsHandle_[8] = TextureManager::Load("numbers/8.png");
	numsHandle_[9] = TextureManager::Load("numbers/9.png");

	RBHandle_ = TextureManager::Load("UI/RB.png");
	LBHandle_ = TextureManager::Load("UI/LB.png");

	pUIHandle_ = TextureManager::Load("UI/P.png");
	lineHandle_ = TextureManager::Load("UI/line.png");

	sprite2DReticle_ =
	    Sprite::Create(reticleHandle_, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	bulletUI_ =
	    Sprite::Create(bulletUIHandle_, Vector2(1100, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	magUI =
	    Sprite::Create(magnetUIHandle_, Vector2(1100, 530), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	numsUI_ =
	    Sprite::Create(numsHandle_[0], Vector2(1230, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	
	numsUIten_ =
	    Sprite::Create(numsHandle_[0], Vector2(1190, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	magNum_ =
	    Sprite::Create(numsHandle_[0], Vector2(1210, 530), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	hpUI_ = 
	    Sprite::Create(hpHandle_, Vector2(50, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	
	hpNum_ =
	    Sprite::Create(numsHandle_[0], Vector2(120, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	
	RBUI_ = 
	    Sprite::Create(RBHandle_, Vector2(1000, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	
	LBUI_ = Sprite::Create(LBHandle_, Vector2(1000, 530), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));

	pUI_ = Sprite::Create(pUIHandle_, Vector2(uiPos, 50), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	pUI_->SetSize(Vector2(40, 40));
	lineUI_ = Sprite::Create(lineHandle_, Vector2(640, 50), Vector4(1, 1, 1, 0.8f), Vector2(0.5, 0.5));
	lineUI_->SetSize(Vector2(600, 20));
	nowBulletNum = startNum;

	canMag = true;
}

Player::~Player() {

	delete sprite2DReticle_;
	delete numsUI_;
	delete numsUIten_;
	delete bulletUI_;
	delete magUI;
	delete magNum_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Update(ViewProjection viewPrpjection) {

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 行列を指定バッファに転送
	worldTransform_.TransferMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// デバッグ用
	float pos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	// キャラクターの移動速さ
	float kCharacterSpeed = 0.2f;

	if (isInhole) {
		kCharacterSpeed = 0.15f;
	}

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 移動限界
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_ = Calculation::VectorAdd(worldTransform_.translation_, move);

	pos[0] = worldTransform_.translation_.x;
	pos[1] = worldTransform_.translation_.y;
	pos[2] = worldTransform_.translation_.z;
	worldTransform_.UpdateMatrix();

	//ImGui::Begin("a");
	//ImGui::SliderFloat3("Player", pos, 0.0f, 1280);
	//worldTransform_.translation_.x = pos[0];
	//worldTransform_.translation_.y = pos[1];
	//worldTransform_.translation_.z = pos[2];
	//ImGui::End();

	//ImGui::Begin("b");
	//ImGui::SliderInt("bullet", &nowBulletNum, 0, 1280);
	//ImGui::End();

	// 旋回
	Rotate();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = Calculation::TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Calculation::Multiply(kDistancePlayerTo3DReticle, Calculation::Normalize(offset));
	// 3Dレティクルの座標設定
	worldTransForm3DReticle_.translation_ = Calculation::VectorAdd(GetWorldPosition(), offset);
	worldTransForm3DReticle_.UpdateMatrix();

	Matrix4x4 matViewport =
	    Calculation::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport = Calculation::Multiply(
	    viewPrpjection.matView, Calculation::Multiply(viewPrpjection.matProjection, matViewport));

	// スプライトの座標
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && canMag) {
			isMagTime = true;
			magTimer = magInterval;
			isInhole = true;
		} else {
			isInhole = false;
		}

		if (isInhole == false && isMagTime == true) {
			if (magTimer == 0) {
				canMag = true;
			} else {
				canMag = false;
				magTimer--;
			}
		}

		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 8.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 8.0f;

		sprite2DReticle_->SetPosition(spritePosition);

		Matrix4x4 matVPV = Calculation::Multiply(
		    Calculation::Multiply(viewPrpjection.matView, viewPrpjection.matProjection),
		    matViewport);
		Matrix4x4 matInverseVPV = Calculation::Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3(float(spritePosition.x), float(spritePosition.y), 0);
		Vector3 posFar = Vector3(float(spritePosition.x), float(spritePosition.y), 1);

		posNear = Calculation::Transform(posNear, matInverseVPV);
		posFar = Calculation::Transform(posFar, matInverseVPV);

		Vector3 padDirection = Calculation::VectorSubtraction(posFar, posNear);
		padDirection = Calculation::Normalize(padDirection);

		const float kDistanceTestObject = 100;

		worldTransForm3DReticle_.translation_ = Calculation::VectorAdd(
		    posNear, Calculation::Multiply(kDistanceTestObject, padDirection));

		worldTransForm3DReticle_.UpdateMatrix();
	}
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		Mouse(viewPrpjection, matViewport);
	}

	if (shotTimer <= 0) {
		if (nowBulletNum > 0 && isInhole == false) {
			Attack();
		}
	} else {
		shotTimer--;
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update(isInhole, GetWorldPosition());
	}
}

void Player::Mouse(ViewProjection viewPrpjection, Matrix4x4 matViewport) {
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));
	Matrix4x4 matVPV = Calculation::Multiply(
	    Calculation::Multiply(viewPrpjection.matView, viewPrpjection.matProjection), matViewport);
	Matrix4x4 matInverseVPV = Calculation::Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

	posNear = Calculation::Transform(posNear, matInverseVPV);
	posFar = Calculation::Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = Calculation::VectorSubtraction(posFar, posNear);
	mouseDirection = Calculation::Normalize(mouseDirection);

	const float kDistanceTestObject = 100;

	worldTransForm3DReticle_.translation_ =
	    Calculation::VectorAdd(posNear, Calculation::Multiply(kDistanceTestObject, mouseDirection));

	worldTransForm3DReticle_.UpdateMatrix();
}

void Player::Rotate() {
	// 回転の速さ
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変換
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	XINPUT_STATE joyState;
	// 弾の速度
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (input_->TriggerKey(DIK_SPACE)) {
			shotTimer = ShotInterval;
			nowBulletNum -= 1;
			const float kBulletSpeed = 1.0f;

			Vector3 velocity = Vector3(0, 0, kBulletSpeed);

			velocity = Calculation::VectorSubtraction(
			    worldTransForm3DReticle_.translation_, GetWorldPosition());
			velocity = Calculation::Normalize(velocity);
			velocity = Calculation::Multiply(kBulletSpeed, velocity);

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity, true);

			bullets_.push_back(newBullet);
		}
	} else {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			shotTimer = ShotInterval;
			nowBulletNum -= 1;
			const float kBulletSpeed = 1.0f;

			Vector3 velocity = Vector3(0, 0, kBulletSpeed);

			velocity = Calculation::VectorSubtraction(
			    worldTransForm3DReticle_.translation_, GetWorldPosition());
			velocity = Calculation::Normalize(velocity);
			velocity = Calculation::Multiply(kBulletSpeed, velocity);

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity, true);

			bullets_.push_back(newBullet);
		}
	}
}

void Player::Draw(ViewProjection viewPrpjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewPrpjection);
	}
	// model_->Draw(worldTransForm3DReticle_, viewPrpjection, reticleHandle_);
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() {
	sprite2DReticle_->Draw();
	bulletUI_->Draw();
	
	numsUIten_->SetTextureHandle(numsHandle_[nowBulletNum / 10]);
	numsUI_->SetTextureHandle(numsHandle_[nowBulletNum % 10]);

	magNum_->SetTextureHandle(numsHandle_[magTimer / 60]);

	if (canMag) {
		magUI->SetTextureHandle(useMagnetUIHandle_);
	} else {
		magUI->SetTextureHandle(magnetUIHandle_);		
	}

	magUI->SetSize(Vector2(96, 96));
	magUI->Draw();
	numsUI_->SetSize(Vector2(64, 64));
	numsUIten_->SetSize(Vector2(64, 64));
	numsUI_->Draw();
	numsUIten_->Draw();
	magNum_->SetSize(Vector2(96, 96));
	magNum_->Draw();

	hpUI_->SetSize(Vector2(96, 96));
	hpUI_->Draw();
	hpNum_->SetTextureHandle(numsHandle_[HP]);
	hpNum_->SetSize(Vector2(96, 96));
	hpNum_->Draw();

	RBUI_->SetSize(Vector2(96, 96));
	RBUI_->Draw();
	LBUI_->SetSize(Vector2(96, 96));
	LBUI_->Draw();

	lineUI_->Draw();
	uiPos += 0.2f;
	pUI_->SetPosition(Vector2(uiPos, 50));
	pUI_->Draw();
}

void Player::MekeBullet(Vector3 pos) {
	Vector3 velocity = {1, 0, 0};
	//pos = GetWorldPosition();
	for (int i = 0; i < 3; i++) {

		if (i == 0) {
			velocity = Vector3(0.1f,0.1f,0);
		} else if (i == 1) {
			velocity = Vector3(-0.1f, 0.1f, 0);
		} else {
			velocity = Vector3(0, -0.1f, 0);
		}

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel_, pos, velocity, false);

		bullets_.push_back(newBullet);

	}
	
}

Vector3 Player::Get3DReticleWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransForm3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransForm3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransForm3DReticle_.matWorld_.m[3][2];

	return worldPos;
}
