#pragma once

/**
 * @file MusicNotesData.h
 * @brief 譜面データ
 * @author 茂木翼
 */

#include <vector>
#include <string>
#include <Note/NoteInformation.h>
#include <Note/MusicInformation.h>
#include <Note/NoteBarInformation.h>

/// <summary>
/// 譜面データ
/// </summary>
struct MusicNotesData {

	//左
	std::vector<NoteInformation> left = {};
	//左中
	std::vector<NoteInformation> leftMiddle = {};
	//右中
	std::vector<NoteInformation> rightMiddle = {};
	//右
	std::vector<NoteInformation> right = {};
	//楽曲情報
	MusicInformation musuicInformation = {};
	//ハンドル
	int32_t handle = -1;
	//譜面のパス
	std::string fullFilePath_;
	//1小節ごとのデータ
	std::vector<NoteBarInformation> newNotesData = {};
};
