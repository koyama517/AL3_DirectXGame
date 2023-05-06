#pragma once
#include<cmath>
#include"Matrix4x4.h"
#include"Vector3.h"

class Calculation {


public:

	static Vector3 VectorAdd(const Vector3 v1, const Vector3 v2);

	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

};
