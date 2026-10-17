#pragma once

/**
 * @file MusicScoreData.h
 * @brief 譜面データ
 * @author 茂木翼
 */

#include <vector>
#include <string>
#include <array>

#include <ScoreData/Difficulty.h>
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
	//譜面のパス
	std::string fullFilePath_="";
	//総ノーツ数
	uint16_t totalNote_ = 0u;

	//1小節ごとのデータ
	//ScoreDataManagerで譜面を読み込むときに使用する
	std::vector<NoteBarInformation> newNotesData = {};

};


/// <summary>
/// 譜面データの存在チェック
/// </summary>
struct ExistMusicScoreData {
	//楽曲名
	std::string musicName="";
	//楽曲ハンドル
	uint32_t musicHandle;
	//ジャケットハンドル
	uint32_t jacketTextureHandle;

	//難易度存在チェック
	std::array<bool, static_cast<uint8_t>(Difficulty::Size)>isExistDifficulty;
	//譜面データ
	std::array<MusicScoreData, static_cast<uint8_t>(Difficulty::Size)>musicScoreData;

	
};