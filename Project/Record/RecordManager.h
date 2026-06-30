#pragma once

#include <string>
#include <array>
#include <map>
#include <Note/NoteJudgement.h>

/// <summary>
/// 記録クラス
/// </summary>
class RecordManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RecordManager() = default;
	
	/// <summary>
	/// 登録
	/// </summary>
	/// <param name="musicName">楽曲名</param>
	/// <param name="level">難易度</param>
	/// <param name="result">結果</param>
	void Register(const std::string& musicName, const std::string& level ,const NoteJudgement::Record& result);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RecordManager() = default;


private:
	//表示するランキングの数
	static const uint8_t RANKING_NUMBER_ = 3u;

public:
	/// <summary>
	/// 結果のランキングデータを取得
	/// </summary>
	/// <param name="musicName">楽曲名</param>
	/// <param name="level">レベル</param>
	/// <returns>結果のランキングデータ</returns>
	std::array<NoteJudgement::Record, RANKING_NUMBER_> GetRankingData(const std::string& musicName, const std::string& level) {
		//まずは楽曲を探す
		auto it = noteJudgementResultMap.find(musicName);
		if (it != noteJudgementResultMap.end()) {

			//レベルで探す
			auto it2 = it->second.find(level);
			if (it2 != it->second.end()) {
				return it2->second.rankingArray;
			}
		}
		else {
			//見つからなかった場合空データを返す
			return {};
		}
	}


private:
	/// <summary>
	/// ランキング
	/// </summary>
	struct JudgementResultRanking {
		//各譜面のランキング上位3位まで
		std::array< NoteJudgement::Record, RANKING_NUMBER_> rankingArray;
	};

private:
	//スコアデータ
	//楽曲名、難易度、結果
	std::map<std::string, std::map<std::string,JudgementResultRanking>> noteJudgementResultMap;


};

