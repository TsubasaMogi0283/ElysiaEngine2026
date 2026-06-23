#pragma once

#include <string>
#include <array>
#include <map>
#include "NotesJudgementResult.h"

/// <summary>
/// 判定結果管理クラス
/// </summary>
class JudgementResultManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	JudgementResultManager() = default;
	
	/// <summary>
	/// 登録
	/// </summary>
	/// <param name="musicName">楽曲名</param>
	/// <param name="level">難易度</param>
	/// <param name="result">結果</param>
	void Register(const std::string& musicName, const std::string& level ,const NotesJudgementResult& result);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~JudgementResultManager() = default;

private:
	//表示するランキングの数
	static const uint8_t RANKING_NUMBER_ = 3u;

private:
	/// <summary>
	/// ランキング
	/// </summary>
	struct JudgementResultRanking {
		//各譜面のランキング上位3位まで
		std::array< NotesJudgementResult, RANKING_NUMBER_> rankingArray;
	};

private:
	//スコアデータ
	//楽曲名、難易度、結果
	std::map<std::string, std::map<std::string,JudgementResultRanking>> noteJudgementResultMap;


};

