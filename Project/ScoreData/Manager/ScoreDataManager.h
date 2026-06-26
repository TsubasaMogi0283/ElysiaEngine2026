#pragma once

#include <vector>
#include <map>
#include <nlohmannJSON/json.hpp>
#include <ScoreData/NoteInformation.h>
#include <ScoreData/MusicScoreData.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia{
	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;
}

/// <summary>
/// 譜面管理クラス
/// </summary>
class ScoreDataManager {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ScoreDataManager();

	/// <summary>
	/// ノーツが流れる処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScoreDataManager() = default;

private:
	/// <summary>
	/// 譜面データ読み込み
	/// </summary>
	/// <param name="path"></param>
	void Load(const std::string& path);

	/// <summary>
	/// 配置
	/// </summary>
	void Place(MusicScoreData& musicNotesData, const nlohmann::json& deserialized);

	/// <summary>
	/// 解凍
	/// </summary>
	static nlohmann::json Deserialize(const std::string& fullFilePath);


public:
	MusicScoreData GetMusicScoreData(const std::string& id,const std::string& level) {
		level;
		MusicScoreData musicScoreData = {};
		//見つけたら代入
		auto it = musicScoreData_.find(id);
		if (it != musicScoreData_.end()) {
			musicScoreData = it->second;
		}
		else {
			//空データ
			musicScoreData = {};
		}

		return musicScoreData;
	}

private:
	//オーディオ
	Elysia::Audio* audio_ = nullptr;

private:
	//譜面が入っているフォルダパス
	const std::string MUSIC_SCORE_PATH_ = "Resources/MusicScore/";

	//各譜面情報
	std::map<std::string,MusicScoreData> musicScoreData_ = {};
	
};

