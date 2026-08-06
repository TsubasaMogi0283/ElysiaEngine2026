#pragma once

/**
 * @file SingleCalculation.h
 * @brief 単体の計算
 * @author 茂木翼
 */
#include <cmath>



/// <summary>
/// 単体の計算
/// </summary>
namespace SingleCalculation{

	

	/// <summary>
	/// コタンジェント
	/// </summary>
	/// <param name="theta_">θ</param>
	/// <returns>値</returns>
	static inline float_t Cot(const float_t& theta) { 
		return (1.0f / std::tanf(theta)); 
	};

	/// <summary>
	/// 線形補間(float)
	/// </summary>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <param name="t">値</param>
	/// <returns>始点から終点までの値</returns>
	static inline float_t Lerp(const float_t& start, const float_t& end, const float_t& t) {
		return (1.0f - t) * start + t * end;
	};

	template <typename Type>

	/// <summary>
	/// 割合を求める(線形補間の逆)
	/// </summary>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <param name="value">値</param>
	/// <returns>割合</returns>
	static inline Type InverseLerp(const Type& start, const Type& end, const Type& value) {
		return static_cast<Type>(value - start) / static_cast<Type>(end - start);
	};
};