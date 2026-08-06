#pragma once

/**
 * @file Model.h
 * @brief ノーツの情報
 * @author 茂木翼
 */

#include <cstdint>
#include <cmath>
#include <Note/BaseNote.h>
#include <Note/NoteType.h>

/// <summary>
/// ノーツの情報
/// </summary>
struct NoteInformation {
	//上下のレーン
	uint8_t place = 0u;
	//ノーツの感覚(音符)
	size_t length = 0u;
	//種類
	NoteType type = NoteType::None;
	//動き出す時間
	float_t startMoveTime = 0.0f;
	//到着時間
	float_t arriveLineTime = 0.0f;
	//動いているときの比率
	float_t moveRatio = 0.0f;

	//判定
	uint8_t judgement = 0u;
	//判定したかどうか
	bool isJudged = false;
	//割り当てられたかどうか
	bool isAssigned = false;
	//オブジェクトプール用の割り当て番号
	int8_t poolIndex = -1;
};