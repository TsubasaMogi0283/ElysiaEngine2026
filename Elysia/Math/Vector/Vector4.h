#pragma once
#include <cmath>
/**
 * @file Vector4.h
 * @brief ベクトル(4D)
 * @author 茂木翼
 */

template <typename Type>

 /// <summary>
 /// ベクトル(4D)
 /// </summary>
struct Vector4 {
	//要素
	Type x;
	Type y;
	Type z;
	Type w;

#pragma region 四則演算

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector4 operator+(const Vector4& other) const {
		Vector4 result = {
			.x = this->x + other.x,
			.y = this->y + other.y,
			.z = this->z + other.z,
			.w = this->w + other.w
		};
		return result;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector4 operator-(const Vector4& other) const {
		Vector4 result = {
			.x = this->x - other.x,
			.y = this->y - other.y,
			.z = this->z - other.z,
			.w = this->w - other.w
		};
		return result;
	}

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector4 operator*(const Vector4& other) const {
		Vector4 result = {
			.x = this->x * other.x,
			.y = this->y * other.y,
			.z = this->z * other.z,
			.w = this->w * other.w
		};
		return result;
	}

	/// <summary>
	/// 除算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector4 operator/(const Vector4& other) const {
		Vector4 result = {
			.x = this->x / other.x,
			.y = this->y / other.y,
			.z = this->z / other.z,
			.w = this->w / other.w
		};
		return result;
	}

#pragma endregion

};