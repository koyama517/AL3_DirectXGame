#include"WorldTransform.h"

void WorldTransform::UpdateMatrix() { 

	matWorld_ = Calculation::MakeAffineMatrix(scale_, rotation_, translation_);
	
	if (parent_) {
		matWorld_ = Calculation::Multiply(matWorld_, parent_->matWorld_);
	}
	
	TransferMatrix();


}
