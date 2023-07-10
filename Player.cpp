#include "Player.h"
#include "ViewProjection.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	input_ = Input::GetInstance();
	worldTransForm3DReticle_.Initialize();

	reticleHandle_ = TextureManager::Load("reticle.png");

	sprite2DReticle_ =
	    Sprite::Create(reticleHandle_, Vector2(0, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
}

Player::~Player() {

	delete sprite2DReticle_;
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
	const float kCharacterSpeed = 0.2f;

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

	ImGui::Begin("a");
	ImGui::SliderFloat3("Player", pos, 0.0f, 1280);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];
	ImGui::End();

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

	/*positionReticle = Calculation::Transform(positionReticle, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));*/

	Matrix4x4 matViewport =
	    Calculation::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport = Calculation::Multiply(
	    viewPrpjection.matView, Calculation::Multiply(viewPrpjection.matProjection, matViewport));

	// スプライトの座標
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

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
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
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

	worldTransForm3DReticle_.translation_ = Calculation::VectorAdd(posNear, 
		Calculation::Multiply(kDistanceTestObject, mouseDirection));

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
		if (input_->PushKey(DIK_SPACE)) {
			const float kBulletSpeed = 1.0f;

			Vector3 velocity = Vector3(0, 0, kBulletSpeed);

			velocity = Calculation::VectorSubtraction(
			    worldTransForm3DReticle_.translation_, GetWorldPosition());
			velocity = Calculation::Normalize(velocity);
			velocity = Calculation::Multiply(kBulletSpeed, velocity);

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			bullets_.push_back(newBullet);
		}
	} else {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

			const float kBulletSpeed = 1.0f;

			Vector3 velocity = Vector3(0, 0, kBulletSpeed);

			velocity = Calculation::VectorSubtraction(
			    worldTransForm3DReticle_.translation_, GetWorldPosition());
			velocity = Calculation::Normalize(velocity);
			velocity = Calculation::Multiply(kBulletSpeed, velocity);

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			bullets_.push_back(newBullet);
		}
	}
}

void Player::Draw(ViewProjection viewPrpjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewPrpjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewPrpjection);
	}
	model_->Draw(worldTransForm3DReticle_, viewPrpjection, reticleHandle_);
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() { sprite2DReticle_->Draw(); }

Vector3 Player::Get3DReticleWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransForm3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransForm3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransForm3DReticle_.matWorld_.m[3][2];

	return worldPos;
}
