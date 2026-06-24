#pragma once
/**
 * @file MusicScoreData.h
 * @brief 譜面データ
 * @author 茂木翼
 */

#include <cstdint>

/// <summary>
/// ノーツの判定結果
/// </summary>
struct NotesJudgementResult {
	//ミス
	uint8_t miss = 0;
	//グッド
	uint8_t good = 0;
	//グレート
	uint8_t great = 0;
	//パーフェクト
	uint8_t perfect = 0;

	//コンボ
	uint8_t combo = 0u;
	//最大コンボ
	uint8_t maxCombo = 0u;
	//総ノーツ数
	uint8_t totalNotes = 0;

	//スコア
	int32_t score = 0;

};