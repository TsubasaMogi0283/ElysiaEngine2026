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

	/// <summary>
	/// 割合を求める(線形補間の逆)
	/// </summary>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <param name="value">値</param>
	/// <returns>割合</returns>
	static inline float_t InverseLerp(const float_t& start, const float_t& end, const float_t& value) {
		//始点と終点が同じだったら計算をしない
		if (start == end) {
			return 0.0f;
		}
		return (value - start) / (end - start);
	};
};