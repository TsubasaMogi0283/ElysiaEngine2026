#pragma once

/**
 * @file Color.h
 * @brief 色
 * @author 茂木翼
 */

//色の早見表
//欲しいものや、使いやすいものが合ったらどんどん追加していく
//https://www.colordic.org/

#include "cstdint"
#include <Vector4.h>
#include <Hsv.h>

/// <summary>
/// 色
/// </summary>
namespace Color {

	//白
	const uint32_t WHITE = 0xFFFFFFFF;
	//黒
	const uint32_t BLACK = 0x000000FF;
	//赤
	const uint32_t RED = 0xFF0000FF;
	//緑
	const uint32_t GREEN = 0x00FF00FF;
	//青
	const uint32_t BLUE = 0x0000FFFF;
	//黄
	const uint32_t YELLOW = 0xFFFF00FF;
	//紫
	const uint32_t PURPLE = 0xFF00FFFF;
	//オレンジ
	const uint32_t ORANGE = 0xFFA500FF;


	//難易度カラー
	//簡単(緑)
	const uint32_t EASY = GREEN;
	//普通(山吹)
	const uint32_t NORMAL = 0xF8B500FF;
	//難しい(赤)
	const uint32_t HARD = 0xFF0000FF;
	//特別(紫)
	const uint32_t SPECIAL = 0x8A2BE2FF;


	/// <summary>
	/// 変換関数
	/// </summary>
	/// <param name="colorCode">カラーコード(RGBA)</param>
	/// <returns>Vector4色情報</returns>
	inline Vector4<float_t> Conevert(const uint32_t& colorCode) {
		
		Vector4<float_t> result = {
			//赤
		   .x = ((colorCode >> 24) & 0xFF) / 255.0f,
		   //緑
		   .y = ((colorCode >> 16) & 0xFF) / 255.0f,
		   //青
		   .z = ((colorCode >> 8) & 0xFF) / 255.0f,
		   //透明度
		   .w = ((colorCode) & 0xFF) / 255.0f

		};

		return result;
	}

	/// <summary>
	/// HSVからRGBに変換
	/// </summary>
	/// <param name="hsv"></param>
	/// <returns></returns>
	inline Vector4<float_t> HsvToRgb(const Hsv& hsv) {

		float_t h = hsv.hue - std::floor(hsv.hue);          // 0〜1にループさせる
		float_t h6 = h * 6.0f;
		int32_t i = static_cast<int32_t>(h6);
		float_t f = h6 - static_cast<float_t>(i);

		float_t p = hsv.value * (1.0f - hsv.saturation);
		float_t q = hsv.value * (1.0f - hsv.saturation * f);
		float_t t = hsv.value * (1.0f - hsv.saturation * (1.0f - f));

		switch (i % 6) {
		case 0:  
			return { hsv.value, t, p, hsv.a };
		case 1:  
			return { q, hsv.value, p, hsv.a };
		case 2:  
			return { p, hsv.value, t, hsv.a };
		case 3:  
			return { p, q, hsv.value, hsv.a };
		case 4:  
			return { t, p, hsv.value, hsv.a };
		default: 
			return { hsv.value, p, q, hsv.a };
		}
	}

}

