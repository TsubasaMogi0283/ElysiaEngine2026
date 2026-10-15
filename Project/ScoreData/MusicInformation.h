#pragma once

/**
 * @file MusicInformation.h
 * @brief 楽曲情報
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
	//難易度値
	std::string difficulty = "";
	//レベル
	int32_t level = 0;
	//BPM
	float_t bpm = 170.0f;
	//オフセット
	float_t offset = 0.0f;
	//id(title+level)
	std::string id = "";
	//楽曲ファイル名
	std::string fileName = "";
};