#pragma once

/**
 * @file Model.h
 * @brief ノーツの情報
 * @author 茂木翼
 */

#include <cstdint>
#include <cmath>
#include <Note/BaseNote.h>

/// <summary>
/// ノーツの情報
/// </summary>
struct NoteInformation {
	//上下のレーン
	uint8_t place;
	//ノーツの感覚(音符)
	uint8_t noteLength;
	//動き出す時間
	float_t startMoveTime = 0.0f;
	//到着時間
	float_t arriveTime = 0.0f;
	//初期の座標
	Vector3 initialPosition = {};
	//現在の座標
	Vector3 currentPosition = {};
	//動いているときの比率
	float_t moveRatio = 0.0f;

	//表示の設定
	bool isDisplay = false;
	//処理が終わったかどうか
	bool isProcessEnd = false;;
	//判定
	uint8_t judgement;
	//判定したかどうか
	bool isJudged = false;

	//ノーツ
	std::shared_ptr<BaseNote> note=nullptr;

};