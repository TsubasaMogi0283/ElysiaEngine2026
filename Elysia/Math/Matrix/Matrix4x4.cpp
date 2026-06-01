#include "Matrix4x4.h"

#include "Vector3.h"
#include "Quaternion.h"
#include "SingleCalculation.h"

Matrix4x4 Matrix4x4::MakeIdentity4x4() {
	Matrix4x4 result = {
		result.m[0][0] = 1.0f,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 1.0f,
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = 1.0f,
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,
	};

	return result;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {
		result.m[0][0] = (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]),
		result.m[0][1] = (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]),
		result.m[0][2] = (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]),
		result.m[0][3] = (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]),

		result.m[1][0] = (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]),
		result.m[1][1] = (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]),
		result.m[1][2] = (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]),
		result.m[1][3] = (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]),

		result.m[2][0] = (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]),
		result.m[2][1] = (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]),
		result.m[2][2] = (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]),
		result.m[2][3] = (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]),

		result.m[3][0] = (m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]),
		result.m[3][1] = (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]),
		result.m[3][2] = (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]),
		result.m[3][3] = (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]),
	};

	return result;

}

Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		result.m[0][0] = scale.x,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = scale.y,
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = scale.z,
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,

	};

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateXMatrix(const float_t& radian) {
	Matrix4x4 result = {

		result.m[0][0] = 1.0f,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = std::cos(radian),
		result.m[1][2] = std::sin(radian),
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = -(std::sin(radian)),
		result.m[2][2] = std::cos(radian),
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,
	};

	return result;

}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(const float_t& radian) {
	Matrix4x4 result = {
		result.m[0][0] = std::cos(radian),
		result.m[0][1] = 0.0f,
		result.m[0][2] = -(std::sin(radian)),
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 1.0f,
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = std::sin(radian),
		result.m[2][1] = 0.0f,
		result.m[2][2] = std::cos(radian),
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,

	};

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(const float_t& radian) {
	Matrix4x4 result = {
		result.m[0][0] = std::cos(radian),
		result.m[0][1] = std::sin(radian),
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = -(std::sin(radian)),
		result.m[1][1] = std::cos(radian),
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = 1.0f,
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,

	};
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateXYZMatrix(const float_t& radianX, const float_t& radianY, const float_t& radianZ) {

	//XYZそれぞれの行列を作る
	Matrix4x4 rotateMatrixX = Matrix4x4::MakeRotateXMatrix(radianX);
	Matrix4x4 rotateMatrixY = Matrix4x4::MakeRotateYMatrix(radianY);
	Matrix4x4 rotateMatrixZ = Matrix4x4::MakeRotateZMatrix(radianZ);

	//まとめる
	Matrix4x4 result = Matrix4x4::Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateMatrix(const Quaternion& quaternion){
	Matrix4x4 result = {};
	float_t w = quaternion.w;
	float_t x = quaternion.x;
	float_t y = quaternion.y;
	float_t z = quaternion.z;

	result.m[0][0] = (w * w) + (x * x) - (y * y) - (z * z);
	result.m[0][1] = 2.0f * (x * y + w * z);
	result.m[0][2] = 2.0f * (x * z - w * y);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (x * y - w * z);
	result.m[1][1] = (w * w) - (x * x) + (y * y) - (z * z);
	result.m[1][2] = 2.0f * (y * z + w * x);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (x * z + w * y);
	result.m[2][1] = 2.0f * (y * z - w * x);
	result.m[2][2] = (w * w) - (x * x) - (y * y) + (z * z);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}


Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;


	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;


	return result;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	//Scale
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(scale);
	//Rotate
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(rotate.x, rotate.y, rotate.z);
	//Translate
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translate);
	//Affine
	Matrix4x4 result = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));

	return result;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m) {
	float_t MatrixFormula =
		+(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
		+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])

		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])
		- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])

		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
		- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
		- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])

		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])

		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])


		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])

		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
		- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
		- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])

		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	Matrix4x4 result = {
		result.m[0][0] = (1 / MatrixFormula) * (
			+(m.m[1][1] * m.m[2][2] * m.m[3][3])
			+ (m.m[1][2] * m.m[2][3] * m.m[3][1])
			+ (m.m[1][3] * m.m[2][1] * m.m[3][2])
			- (m.m[1][3] * m.m[2][2] * m.m[3][1])
			- (m.m[1][2] * m.m[2][1] * m.m[3][3])
			- (m.m[1][1] * m.m[2][3] * m.m[3][2])),

		result.m[0][1] = (1 / MatrixFormula) * (
			-(m.m[0][1] * m.m[2][2] * m.m[3][3])
			- (m.m[0][2] * m.m[2][3] * m.m[3][1])
			- (m.m[0][3] * m.m[2][1] * m.m[3][2])
			+ (m.m[0][3] * m.m[2][2] * m.m[3][1])
			+ (m.m[0][2] * m.m[2][1] * m.m[3][3])
			+ (m.m[0][1] * m.m[2][3] * m.m[3][2])),


		result.m[0][2] = (1 / MatrixFormula) * (
			+(m.m[0][1] * m.m[1][2] * m.m[3][3])
			+ (m.m[0][2] * m.m[1][3] * m.m[3][1])
			+ (m.m[0][3] * m.m[1][1] * m.m[3][2])
			- (m.m[0][3] * m.m[1][2] * m.m[3][1])
			- (m.m[0][2] * m.m[1][1] * m.m[3][3])
			- (m.m[0][1] * m.m[1][3] * m.m[3][2])),

		result.m[0][3] = (1 / MatrixFormula) * (
			-(m.m[0][1] * m.m[1][2] * m.m[2][3])
			- (m.m[0][2] * m.m[1][3] * m.m[2][1])
			- (m.m[0][3] * m.m[1][1] * m.m[2][2])
			+ (m.m[0][3] * m.m[1][2] * m.m[2][1])
			+ (m.m[0][2] * m.m[1][1] * m.m[2][3])
			+ (m.m[0][1] * m.m[1][3] * m.m[2][2])),



		result.m[1][0] = (1 / MatrixFormula) * (
			-(m.m[1][0] * m.m[2][2] * m.m[3][3])
			- (m.m[1][2] * m.m[2][3] * m.m[3][0])
			- (m.m[1][3] * m.m[2][0] * m.m[3][2])
			+ (m.m[1][3] * m.m[2][2] * m.m[3][0])
			+ (m.m[1][2] * m.m[2][0] * m.m[3][3])
			+ (m.m[1][0] * m.m[2][3] * m.m[3][2])),

		result.m[1][1] = (1 / MatrixFormula) * (
			+(m.m[0][0] * m.m[2][2] * m.m[3][3])
			+ (m.m[0][2] * m.m[2][3] * m.m[3][0])
			+ (m.m[0][3] * m.m[2][0] * m.m[3][2])
			- (m.m[0][3] * m.m[2][2] * m.m[3][0])
			- (m.m[0][2] * m.m[2][0] * m.m[3][3])
			- (m.m[0][0] * m.m[2][3] * m.m[3][2])),

		result.m[1][2] = (1 / MatrixFormula) * (
			-(m.m[0][0] * m.m[1][2] * m.m[3][3])
			- (m.m[0][2] * m.m[1][3] * m.m[3][0])
			- (m.m[0][3] * m.m[1][0] * m.m[3][2])
			+ (m.m[0][3] * m.m[1][2] * m.m[3][0])
			+ (m.m[0][2] * m.m[1][0] * m.m[3][3])
			+ (m.m[0][0] * m.m[1][3] * m.m[3][2])),

		result.m[1][3] = (1 / MatrixFormula) * (
			+(m.m[0][0] * m.m[1][2] * m.m[2][3])
			+ (m.m[0][2] * m.m[1][3] * m.m[2][0])
			+ (m.m[0][3] * m.m[1][0] * m.m[2][2])
			- (m.m[0][3] * m.m[1][2] * m.m[2][0])
			- (m.m[0][2] * m.m[1][0] * m.m[2][3])
			- (m.m[0][0] * m.m[1][3] * m.m[2][2])),



		result.m[2][0] = (1 / MatrixFormula) * (
			+(m.m[1][0] * m.m[2][1] * m.m[3][3])
			+ (m.m[1][1] * m.m[2][3] * m.m[3][0])
			+ (m.m[1][3] * m.m[2][0] * m.m[3][1])
			- (m.m[1][3] * m.m[2][1] * m.m[3][0])
			- (m.m[1][1] * m.m[2][0] * m.m[3][3])
			- (m.m[1][0] * m.m[2][3] * m.m[3][1])),

		result.m[2][1] = (1 / MatrixFormula) * (
			-(m.m[0][0] * m.m[2][1] * m.m[3][3])
			- (m.m[0][1] * m.m[2][3] * m.m[3][0])
			- (m.m[0][3] * m.m[2][0] * m.m[3][1])
			+ (m.m[0][3] * m.m[2][1] * m.m[3][0])
			+ (m.m[0][1] * m.m[2][0] * m.m[3][3])
			+ (m.m[0][0] * m.m[2][3] * m.m[3][1])),

		result.m[2][2] = (1 / MatrixFormula) * (
			+(m.m[0][0] * m.m[1][1] * m.m[3][3])
			+ (m.m[0][1] * m.m[1][3] * m.m[3][0])
			+ (m.m[0][3] * m.m[1][0] * m.m[3][1])
			- (m.m[0][3] * m.m[1][1] * m.m[3][0])
			- (m.m[0][1] * m.m[1][0] * m.m[3][3])
			- (m.m[0][0] * m.m[1][3] * m.m[3][1])),

		result.m[2][3] = (1 / MatrixFormula) * (
			-(m.m[0][0] * m.m[1][1] * m.m[2][3])
			- (m.m[0][1] * m.m[1][3] * m.m[2][0])
			- (m.m[0][3] * m.m[1][0] * m.m[2][1])
			+ (m.m[0][3] * m.m[1][1] * m.m[2][0])
			+ (m.m[0][1] * m.m[1][0] * m.m[2][3])
			+ (m.m[0][0] * m.m[1][3] * m.m[2][1])),


		result.m[3][0] = (1 / MatrixFormula) * (
			-(m.m[1][0] * m.m[2][1] * m.m[3][2])
			- (m.m[1][1] * m.m[2][2] * m.m[3][0])
			- (m.m[1][2] * m.m[2][0] * m.m[3][1])
			+ (m.m[1][2] * m.m[2][1] * m.m[3][0])
			+ (m.m[1][1] * m.m[2][0] * m.m[3][2])
			+ (m.m[1][0] * m.m[2][2] * m.m[3][1])),


		result.m[3][1] = (1 / MatrixFormula) * (
			+(m.m[0][0] * m.m[2][1] * m.m[3][2])
			+ (m.m[0][1] * m.m[2][2] * m.m[3][0])
			+ (m.m[0][2] * m.m[2][0] * m.m[3][1])
			- (m.m[0][2] * m.m[2][1] * m.m[3][0])
			- (m.m[0][1] * m.m[2][0] * m.m[3][2])
			- (m.m[0][0] * m.m[2][2] * m.m[3][1])),

		result.m[3][2] = (1 / MatrixFormula) * (
			-(m.m[0][0] * m.m[1][1] * m.m[3][2])
			- (m.m[0][1] * m.m[1][2] * m.m[3][0])
			- (m.m[0][2] * m.m[1][0] * m.m[3][1])
			+ (m.m[0][2] * m.m[1][1] * m.m[3][0])
			+ (m.m[0][1] * m.m[1][0] * m.m[3][2])
			+ (m.m[0][0] * m.m[1][2] * m.m[3][1])),

		result.m[3][3] = (1 / MatrixFormula) * (
			+(m.m[0][0] * m.m[1][1] * m.m[2][2])
			+ (m.m[0][1] * m.m[1][2] * m.m[2][0])
			+ (m.m[0][2] * m.m[1][0] * m.m[2][1])
			- (m.m[0][2] * m.m[1][1] * m.m[2][0])
			- (m.m[0][1] * m.m[1][0] * m.m[2][2])
			- (m.m[0][0] * m.m[1][2] * m.m[2][1])),
	};

	return result;
}

Matrix4x4 Matrix4x4::MakePerspectiveFovMatrix(const float_t& fovY, const float_t& aspectRatio, const float_t& nearClip, const float_t& farClip) {
	float_t theta = fovY / 2.0f;

	Matrix4x4 result = {
		result.m[0][0] = (1.0f / aspectRatio) * SingleCalculation::Cot(theta),
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = SingleCalculation::Cot(theta),
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = farClip / (farClip - nearClip),
		result.m[2][3] = 1.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip),
		result.m[3][3] = 0.0f,
	};

	return result;
}

Matrix4x4 Matrix4x4::MakeOrthographicMatrix(const float_t& left, const float_t& top, const float_t& right, const float_t& bottom, const float_t& nearClip, const float_t& farClip) {
	Matrix4x4 result = {
		result.m[0][0] = 2.0f / (right - left),
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 2.0f / (top - bottom),
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = (1.0f / farClip - nearClip),
		result.m[2][3] = 0.0f,

		result.m[3][0] = (left + right) / (left - right),
		result.m[3][1] = (top + bottom) / (bottom - top),
		result.m[3][2] = nearClip / (nearClip - farClip),
		result.m[3][3] = 1.0f,
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeTransposeMatrix(const Matrix4x4& m) {
	Matrix4x4 result = {
		result.m[0][0] = m.m[0][0],
		result.m[0][1] = m.m[1][0],
		result.m[0][2] = m.m[2][0],
		result.m[0][3] = m.m[3][0],

		result.m[1][0] = m.m[0][1],
		result.m[1][1] = m.m[1][1],
		result.m[1][2] = m.m[2][1],
		result.m[1][3] = m.m[3][1],

		result.m[2][0] = m.m[0][2],
		result.m[2][1] = m.m[1][2],
		result.m[2][2] = m.m[2][2],
		result.m[2][3] = m.m[3][2],

		result.m[3][0] = m.m[0][3],
		result.m[3][1] = m.m[1][3],
		result.m[3][2] = m.m[2][3],
		result.m[3][3] = m.m[3][3],

	};
	return result;
}
