#pragma once

/**
 * @file NoteType.h
 * @brief ノーツのタイプ
 * @author 茂木翼
 */

/// <summary>
/// ノーツタイプ
/// </summary>
enum class NoteType {
	//無し
	None,
	//通常
	NormalTap,
	//開始ハイパスロング
	HiPassLongStart,
	//開始ローパスロング
	LowPassLongStart,
	//開始8分トランスゲートロング
	TranceGate8LongStart,
	//開始16分トランスゲートロング
	TranceGate16LongStart,
	//終了ロング
	LongEnd,

	//サイズ
	Size,
};