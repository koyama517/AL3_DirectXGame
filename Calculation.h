#pragma once
#include<cmath>
#include"Matrix4x4.h"
#include"Vector3.h"
#include<assert.h>

class Calculation {


public:

	static Vector3 Add(const float v1, const Vector3 v2) { 
		Vector3 result;

		result.x = v1 + v2.x;
		result.y = v1 + v2.y;
		result.z = v1 + v2.z;

		return result;
	}

	static Vector3 VectorAdd(const Vector3 v1, const Vector3 v2);
	static Vector3 VectorSubtraction(const Vector3 v1, const Vector3 v2);
	static Vector3 Multiply(const float v1, const Vector3& v2);

	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
	
	static float Dot(const Vector3& v1, const Vector3& v2);
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	static Matrix4x4 MakeViewportMatrix(
	    float left, float top, float widte, float height, float minDepth, float maxDepth) {
		Matrix4x4 result;

		result.m[0][0] = widte / 2;
		result.m[0][1] = 0;
		result.m[0][2] = 0;
		result.m[0][3] = 0;

		result.m[1][0] = 0;
		result.m[1][1] = -height / 2;
		result.m[1][2] = 0;
		result.m[1][3] = 0;

		result.m[2][0] = 0;
		result.m[2][1] = 0;
		result.m[2][2] = maxDepth - minDepth;
		result.m[2][3] = 0;

		result.m[3][0] = left + widte / 2;
		result.m[3][1] = top + height / 2;
		result.m[3][2] = minDepth;
		result.m[3][3] = 1;

		return result;
	}

	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result;

		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		           vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		           vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		           vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		          vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;

		return result;
	};
};
