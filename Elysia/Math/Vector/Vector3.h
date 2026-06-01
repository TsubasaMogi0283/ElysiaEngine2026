#pragma once

/**
 * @file Vector3.h
 * @brief ベクトル(3D)
 * @author 茂木翼
 */

#include <cmath>
#include <algorithm>
#include "SingleCalculation.h"


/// <summary>
/// 行列4x4
/// </summary>
struct Matrix4x4;

/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion;

/// <summary>
/// ベクトル(3D)
/// </summary>
struct Vector3 {
	
	// 要素
	float_t x;
	float_t y;
	float_t z;

#pragma region 四則演算

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector3 operator+(const Vector3& other) const {
		Vector3 result = {
			.x = this->x + other.x,
			.y = this->y + other.y,
			.z = this->z + other.z
		};
		return result;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector3 operator-(const Vector3& other) const {
		Vector3 result = {
			.x = this->x - other.x,
			.y = this->y - other.y,
			.z = this->z - other.z
		};
		return result;
	}

	/// <summary>
	/// 乗算(Vector3*Vector3)
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector3 operator*(const Vector3& other) const {
		Vector3 result = {
			.x = this->x * other.x,
			.y = this->y * other.y,
			.z = this->z * other.z
		};
		return result;
	}

	/// <summary>
	/// 乗算(Vector3*float)
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector3 operator*(const float_t& other) const {
		Vector3 result = {
			.x = this->x * other,
			.y = this->y * other,
			.z = this->z * other
		};
		return result;
	}


	/// <summary>
	/// 除算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector3 operator/(const Vector3& other) const {
		Vector3 result = {
			.x = this->x / other.x,
			.y = this->y / other.y,
			.z = this->z / other.z
		};
		return result;
	}
#pragma endregion

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	static inline Vector3 Normalize(const Vector3& v) {
		//長さを求める
		float_t length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		//仮で入れる
		float_t newX = v.x;
		float_t newY = v.y;
		float_t newZ = v.z;
		//0除算を防ぐ
		if (length != 0.0f) {
			newX = v.x / length;
			newY = v.y / length;
			newZ = v.z / length;

		}
		//最終結果
		Vector3 result = {
			.x = newX,
			.y = newY,
			.z = newZ,
		};
		return result;
	}


	/// <summary>
	/// 線形補間(Vector3)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">t</param>
	/// <returns></returns>
	static inline Vector3 Lerp(const Vector3& v1, const Vector3& v2, const float_t& t) {
		Vector3 result = {
			.x = (1.0f - t) * v1.x + t * v2.x,
			.y = (1.0f - t) * v1.y + t * v2.y,
			.z = (1.0f - t) * v1.z + t * v2.z,
		};

		return result;
	};

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">t</param>
	/// <returns></returns>
	static inline Vector3 Slerp(const Vector3& v1, const Vector3& v2, const float_t& t) {
		float_t newT = std::clamp(t, 0.0f, 1.0f);

		//正規化
		Vector3 normalizeV1 = Normalize(v1);
		Vector3 normalizeV2 = Normalize(v2);

		//内積
		float_t dot = SingleCalculation::Dot(normalizeV1, normalizeV2);

		//θを求める
		float_t theta = std::acosf(dot) * newT;

		//差分
		Vector3 subtractVector = v2- v1;

		//差分にnewTをかける
		Vector3 newSubtractVector = {
			subtractVector.x * newT,
			subtractVector.y * newT,
			subtractVector.z * newT };

		//正規化
		Vector3 relativeVector = Normalize(newSubtractVector);

		Vector3 result = {
			v1.x * std::cos(theta) + relativeVector.x * std::sin(theta),
			v1.y * std::cos(theta) + relativeVector.y * std::sin(theta),
			v1.z * std::cos(theta) + relativeVector.z * std::sin(theta)
		};

		return result;
	};

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 result = {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
		};
		return result;
	};


	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static inline Vector3 Project(const Vector3& v1, const Vector3& v2) {
		//内積
		float_t dot = SingleCalculation::Dot(v1, v2);
		//長さ
		float_t length = SingleCalculation::Length(v2);
		float_t t = dot / (length * length);
		//最終結果
		Vector3 result = {
			.x = t * v2.x,
			.y = t * v2.y,
			.z = t * v2.z
		};
		return result;
	};

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="m">行列</param>
	/// <returns></returns>
	static inline Vector3 TransformCalculation(const Vector3& v, const Matrix4x4& m);


	/// <summary>
	/// Catmull-Romスプライン曲線(3D)
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static inline Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, const float_t& t) {
		//半分
		const float_t HALF = 1.0f / 2.0f;
		//計算
		Vector3 result = {
			.x = HALF * (
			(-1.0f * p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * (t * t * t) +
			(2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * (t * t) +
			(-1.0f * p0.x + p2.x) * t +
			2.0f * p1.x),

			.y = HALF * (
			(-1.0f * p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * (t * t * t) +
			(2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * (t * t) +
			(-1.0f * p0.y + p2.y) * t +
			2.0f * p1.y),

			.z = HALF * (
			(-1.0f * p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * (t * t * t) +
			(2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * (t * t) +
			(-1.0f * p0.z + p2.z) * t +
			2.0f * p1.z),
		};
		return result;
	};

	/// <summary>
	/// ベクトルをQuaternionで回転させた結果のベクトルを求める
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static inline Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

};