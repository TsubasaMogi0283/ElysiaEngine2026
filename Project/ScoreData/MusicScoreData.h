#pragma once

/**
 * @file MusicScoreData.h
 * @brief 譜面データ
 * @author 茂木翼
 */

#include <vector>
#include <string>
#include <ScoreData/MusicInformation.h>
#include <ScoreData/NoteInformation.h>
#include <ScoreData/NoteBarInformation.h>

/// <summary>
/// 譜面データ
/// </summary>
struct MusicScoreData {

	//上情報
	std::vector<NoteInformation> upInformation = {};
	//下情報
	std::vector<NoteInformation> downInformation = {};

	//楽曲情報
	MusicInformation musicInformation = {};
	//楽曲ハンドル
	int32_t musicHandle = -1;
	//譜面のパス
	std::string fullFilePath_="";
	//総ノーツ数
	uint16_t totalNote_ = 0u;

	//1小節ごとのデータ
	std::vector<NoteBarInformation> newNotesData = {};
};
