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
		uint32_t score = 0u;

	};

	/// <summary>
	/// 判定の選択
	/// </summary>
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

		//サイズ
		Size
	};

	/// <summary>
	/// 時間
	/// </summary>
	namespace Time {
		//パーフェクトの判定時間
		constexpr float_t PERFECT = 0.05f;
		//グレートの判定時間
		constexpr float_t GREAT = 0.1f;
		//グッドの判定時間
		constexpr float_t GOOD = 0.15f;
		//ミスの判定時間
		constexpr float_t MISS = 0.2f;
	}

	/// <summary>
	/// 基本スコア
	/// </summary>
	namespace BasicScore {
		//パーフェクト
		constexpr uint32_t PERFECT = 1000u;
		//グレート
		constexpr uint32_t GREAT = 500u;
		//グッド
		constexpr uint32_t GOOD = 200u;
	}
};

