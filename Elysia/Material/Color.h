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
	//簡単(水)
	const uint32_t EASY = 0x7FFF00FF;
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
	inline Vector4 Conevert(const uint32_t& colorCode) {
		
		Vector4 result = {
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

}

