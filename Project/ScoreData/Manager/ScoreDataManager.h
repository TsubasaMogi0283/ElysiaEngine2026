#pragma once

#include <vector>
#include <map>
#include <filesystem>
#include <string>

#include <nlohmannJSON/json.hpp>
#include <ScoreData/NoteInformation.h>
#include <ScoreData/MusicScoreData.h>
#include <ScoreData/Difficulty.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia{
	
	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;
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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScoreDataManager() = default;

private:
	/// <summary>
	/// 譜面データ読み込み
	/// </summary>
	/// <param name="path"></param>
	MusicScoreData Load(const std::string& path);

	/// <summary>
	/// 配置
	/// </summary>
	void Place(MusicScoreData& musicNotesData, const nlohmann::json& deserialized);

	/// <summary>
	/// 解凍
	/// </summary>
	static nlohmann::json Deserialize(const std::string& fullFilePath);

	/// <summary>
	/// 全てのフォルダの名前を取得する
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::vector<std::string>GetFolderName(const std::filesystem::path& path);

public:

	/// <summary>
	/// 楽曲データを取得
	/// </summary>
	/// <param name="title">楽曲名</param>
	/// <param name="difficulty">難易度</param>
	/// <returns>楽曲データ</returns>
	MusicScoreData GetMusicScoreData(const std::string& title,const std::string& difficulty) {
		MusicScoreData musicScoreData = {};
		//見つけたら代入
		for (size_t i = 0u; i < existScoreDataVector_.size(); i++) {
			for (size_t j = 0u; j < existScoreDataVector_[i].musicScoreData.size(); j++) {
				if (existScoreDataVector_[i].musicScoreData[j].musicInformation.id == title + "_" + difficulty) {
					return existScoreDataVector_[i].musicScoreData[j];
				}
			}
		}

		return musicScoreData;
	}

	/// <summary>
	/// サンプル楽曲データの取得
	/// </summary>
	/// <returns>サンプル</returns>
	MusicScoreData GetSampleMusicScoreData() {
		MusicScoreData musicScoreData = {};
		//見つけたら代入
		for (size_t i = 0u; i < existScoreDataVector_.size(); i++) {
			for (size_t j = 0u; j < existScoreDataVector_[i].musicScoreData.size(); j++) {
				if (existScoreDataVector_[i].musicScoreData[j].musicInformation.id == "Construction_Easy") {
					return existScoreDataVector_[i].musicScoreData[j];
				}
			}
		}

		return musicScoreData;
	}

	/// <summary>
	/// 楽曲の存在チェック
	/// </summary>
	/// <returns></returns>
	inline std::vector<ExistMusicScoreData> GetExistScoreData()const {
		return existScoreDataVector_;
	}

private:
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;

private:
	//譜面が入っているフォルダパス
	const std::string MUSIC_SCORE_PATH_ = "Resources/MusicScore";

private:

	//譜面データチェック
	std::vector<ExistMusicScoreData>existScoreDataVector_ = {};
	
};

