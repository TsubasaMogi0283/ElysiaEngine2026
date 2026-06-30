#pragma once
/**
 * @file NoteJudgement.h
 * @brief ノーツの判定
 * @author 茂木翼
 */

#include <cstdint>

/// <summary>
/// ノーツの判定
/// </summary>
namespace NoteJudgement {

	/// <summary>
	/// 判定記録
	/// </summary>
	struct Record {
		//ミス
		uint8_t miss = 0u;
		//グッド
		uint8_t good = 0u;
		//グレート
		uint8_t great = 0u;
		//パーフェクト
		uint8_t perfect = 0u;

		//コンボ
		uint8_t combo = 0u;
		//最大コンボ
		uint8_t maxCombo = 0u;
		//総ノーツ数
		uint8_t total = 0u;

		//スコア
		int32_t score = 0;

	};

	//判定の列挙
	enum Selection {
		//無し
		None,
		//ミス
		Miss,
		//グッド
		Good,
		//グレート
		Great,
		//パーフェクト
		Perfect,

		//列挙体のサイズ
		Size
	};
};

