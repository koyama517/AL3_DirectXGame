#include"WorldTransform.h"

void WorldTransform::UpdateMatrix() { 

	matWorld_ = Calculation::MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();

}
