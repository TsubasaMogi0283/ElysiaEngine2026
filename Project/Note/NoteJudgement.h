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
		uint16_t miss = 0u;
		//グッド
		uint16_t good = 0u;
		//グレート
		uint16_t great = 0u;
		//パーフェクト
		uint16_t perfect = 0u;

		//コンボ
		uint16_t combo = 0u;
		//最大コンボ
		uint16_t maxCombo = 0u;
		//総ノーツ数
		uint16_t totalNotes = 0u;

		//スコア
		uint32_t score = 0u;
		//ゲージ(達成度)
		float_t gaugeRatio = 0.0f;
	};

	/// <summary>
	/// 判定の選択
	/// </summary>
	enum class Selection {
		//ミス
		Miss,
		//グッド
		Good,
		//グレート
		Great,
		//パーフェクト
		Perfect,
		//サイズ
		Size,

		//無し
		None,
	};

	/// <summary>
	/// 時間
	/// </summary>
	namespace Time {
		//パーフェクトの判定時間
		const float_t PERFECT = 0.1f;
		//グレートの判定時間
		const float_t GREAT = 0.15f;
		//グッドの判定時間
		const float_t GOOD = 0.2f;
		//ミスの判定時間
		const float_t MISS = 0.25f;
	}

	/// <summary>
	/// 基本スコア
	/// </summary>
	namespace BasicScore {
		//パーフェクト
		const uint32_t PERFECT = 1000u;
		//グレート
		const uint32_t GREAT = 500u;
		//グッド
		const uint32_t GOOD = 200u;
	}
};

