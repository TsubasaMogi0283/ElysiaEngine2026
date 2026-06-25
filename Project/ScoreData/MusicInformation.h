#pragma once

/**
 * @file Model.h
 * @brief モデル
 * @author 茂木翼
 */

#include <cmath>
#include <string>

/// <summary>
/// 楽曲情報
/// </summary>
struct MusicInformation {
	//タイトル
	std::string title = "";
	//作曲者
	std::string produce = "";
	//レベル
	std::string level = "";
	//難易度値
	int32_t difficulty = 0;
	//BPM
	float_t bpm = 170.0f;
	//オフセット
	float_t offset = 0.0f;
	//id(title+level)
	std::string id = "";
	//楽曲ファイル名
	std::string fileName = "";
	//楽曲ハンドル
	uint32_t musicHandle = 0u;
};