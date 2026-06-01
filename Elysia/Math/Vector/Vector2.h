#pragma once

/**
 * @file Vector2.h
 * @brief ベクトル(2D)
 * @author 茂木翼
 */

#include <cmath>

/// <summary>
/// ベクトル(2D)
/// </summary>
struct Vector2 {
	// 要素
	float_t x;
	float_t y;

#pragma region 四則演算

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector2 operator+(const Vector2& other) const {
		Vector2 result = {
			.x = this->x + other.x,
			.y = this->y + other.y
		};
		return result;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector2 operator-(const Vector2& other) const {
		Vector2 result = {
			.x = this->x - other.x,
			.y = this->y - other.y
		};
		return result;
	}

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector2 operator*(const Vector2& other) const {
		Vector2 result = {
			.x = this->x * other.x,
			.y = this->y * other.y
		};
		return result;
	}

	/// <summary>
	/// 除算
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Vector2 operator/(const Vector2& other) const {
		Vector2 result = {
			.x = this->x / other.x,
			.y = this->y / other.y
		};
		return result;
	}

#pragma endregion

	/// <summary>
	/// 正規化(Vector2)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static inline Vector2 Normalize(const Vector2& v) {
		//長さを求める
		float_t length = sqrtf(v.x * v.x + v.y * v.y);
		//仮で入れる
		float_t tmpX = v.x;
		float_t tmpY = v.y;

		//0除算を防ぐ
		if (length != 0.0f) {
			tmpX = v.x / length;
			tmpY = v.y / length;
		}
		//最終結果
		Vector2 result = {
			.x = tmpX,
			.y = tmpY,
		};

		return result;
	};

	/// <summary>
	/// 線形補間(Vector2)
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static inline Vector2 Lerp(const Vector2& v1, const Vector2& v2, const float_t& t) {
		Vector2 result = {
			.x = (1.0f - t) * v1.x + t * v2.x,
			.y = (1.0f - t) * v1.y + t * v2.y,
		};
		return result;
	};

	/// <summary>
	/// 長さを求める(Vector2)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>長さ</returns>
	static inline float_t Length(const Vector2& v) {
		return sqrtf(v.x * v.x + v.y * v.y);
	};

	/// <summary>
	/// 内積(Vector2)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns>結果</returns>
	static inline float_t Dot(const Vector2& v1, const Vector2& v2) {
		return (v1.x * v2.x + v1.y * v2.y);
	};
};