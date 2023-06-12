#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Calculation.h"
#include "Input.h"

class RailCamera {
public:

	void Initialize(Vector3 position, Vector3 rad);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;
};
