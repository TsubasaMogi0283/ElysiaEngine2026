#include "ScoreDataManager.h"

#include <fstream>
#include <magic_enum/magic_enum.hpp>
#include <Audio.h>
#include <TextureManager.h>
#include <ScoreData/MusicScoreData.h>
#include <StringOption.h>

ScoreDataManager::ScoreDataManager(){
	//インスタンスの取得
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void ScoreDataManager::Initialize(){

	///フォルダの名前を取得
	std::vector<std::string>musicNameVector= GetFolderName(MUSIC_SCORE_PATH_);

	
	//楽曲数分サイズを変更する
	existScoreDataVector_.resize(musicNameVector.size());

	//基本的には譜面データはJSONでやる
	for (uint8_t i = 0u; i < musicNameVector.size(); i++) {
		
		//楽曲名を記録
		existScoreDataVector_[i].musicName = musicNameVector[i];

		//楽曲の読み込み
		const std::string MUSIC_EXTENSION = StringOption::FindExtension(MUSIC_SCORE_PATH_ + "/" + existScoreDataVector_[i].musicName, existScoreDataVector_[i].musicName);
		const std::string MUSIC_FULL_FILE_PATH = MUSIC_SCORE_PATH_ + "/" + existScoreDataVector_[i].musicName + "/" + existScoreDataVector_[i].musicName + MUSIC_EXTENSION;
		existScoreDataVector_[i].musicHandle = audio_->Load(MUSIC_FULL_FILE_PATH);

		//ジャケットの読み込み
		const std::string JACKET_EXTENSION = StringOption::FindExtension(MUSIC_SCORE_PATH_ + "/" + existScoreDataVector_[i].musicName, existScoreDataVector_[i].musicName + "_Jacket");
		const std::string TEXTURE_FULL_FILE_PATH = MUSIC_SCORE_PATH_ + "/" + existScoreDataVector_[i].musicName + "/" + existScoreDataVector_[i].musicName + "_Jacket" + JACKET_EXTENSION;
		existScoreDataVector_[i].jacketTextureHandle = textureManager_->Load(TEXTURE_FULL_FILE_PATH);


		for (uint8_t j = 0u; j < static_cast<uint8_t>(Difficulty::Size); j++) {
			//例:HighwaySunset/HighwaySunset_Easy.json
			//名前を取り出す
			std::string_view difficulty = magic_enum::enum_name(static_cast<Difficulty>(i));
			//std::stringに変換
			std::string convertDifficulty(difficulty);
			std::string scorePath = musicNameVector[i] + "/" + musicNameVector[i] + "_" + convertDifficulty + ".json";
			
			//読み込み出来たら記録
			//パスの結合
			std::string fullFilePath = MUSIC_SCORE_PATH_ + "/" + scorePath;

			//見つからなかったら譜面データを読み込み
			if (std::filesystem::exists(fullFilePath)) {
				existScoreDataVector_[i].isExistDifficulty[j] = true;
				existScoreDataVector_[i].musicScoreData[j] = Load(fullFilePath);
			}
			else {
				existScoreDataVector_[i].isExistDifficulty[j] = false;
			}
		}
	}
}

MusicScoreData ScoreDataManager::Load(const std::string& path) {




	//Load("Construction/Construction_Easy.json");
	//Load("Construction/Construction_Normal.json");
	//Load("Construction/Construction_Hard.json");
	//Load("Construction/Construction_Special.json");
	//
	//
	////パスの結合
	//std::string fullFilePath = "Resources/MusicScore"+"/" + "Construction/Construction_Special.json";
	
	
	//JSON文字列から解凍したデータ
	nlohmann::json deserialized = Deserialize(path);
	
	//楽曲情報を記録
	//タイトル
	const std::string TITLE = deserialized["title"].get<std::string>();
	//作曲
	const std::string PRODUCE = deserialized["produce"].get<std::string>();
	//難易度
	const std::string DIFFICULTY = deserialized["difficulty"].get<std::string>();
	//レベル
	const std::string LEVEL = deserialized["level"].get<std::string>();
	//BPM
	const std::string BPM = deserialized["bpm"].get<std::string>();
	//オフセット
	const std::string OFFSET = deserialized["offset"].get<std::string>();
	//ID
	const std::string ID = TITLE + "_" + DIFFICULTY;
	//楽曲ファイル名
	const std::string MUSIC_FILE_NAME = deserialized["filename"].get<std::string>();
	
	//ハンドルとパスを記録
	MusicScoreData musicNotesData = {};
	const MusicInformation MUSIC_INFORMATION = {
		.title = TITLE,
		.produce = PRODUCE,
		.difficulty = DIFFICULTY,
		.level = std::atoi( LEVEL.c_str()),
		.bpm = static_cast<float_t>(std::atof(BPM.c_str())),
		.offset = static_cast<float_t>(std::atof(OFFSET.c_str())),
		.id = ID,
		.fileName = MUSIC_FILE_NAME,
		
	};
	//挿入
	musicNotesData.musicInformation = MUSIC_INFORMATION;
	musicNotesData.fullFilePath_ = path;

	//ノーツの配置
	Place(musicNotesData, deserialized);

	//返す
	return musicNotesData;
}

void ScoreDataManager::Place(MusicScoreData& musicNotesData, const nlohmann::json& deserialized) {
	if (!deserialized.contains("data") || !deserialized["data"].is_array()) {
		//見つからないか配列じゃない
		assert(0);
		return;
	}

	//データを見ていく
	for (const auto& section : deserialized["data"]) {
		NoteBarInformation noteBar;

		//ノーツ情報
		if (!section.contains("notes") || !section["notes"].is_array()) {
			continue;
		}
		const auto& notesArray = section["notes"];
		noteBar.notesLane.reserve(notesArray.size());

		//行で見ていく
		//合計上下で2つ
		for (const auto& row : notesArray) {
			if (!row.is_array() || row.size() < 2u) {
				continue;
			};
			//ノーツ情報を記録
			NoteLane::Information noteLane = {
				//上
				.upNote = row[0].get<NoteType>(),
				//下
				.downNote = row[1].get<NoteType>(),
			};
			//挿入
			noteBar.notesLane.push_back(noteLane);
		}

		//BPM
		if (section.contains("bpm") && section["bpm"].is_number()) {
			noteBar.bpm = section["bpm"].get<float_t>();
		}
		//無かったらエラー
		else {
			assert(0);
		}
		//挿入
		musicNotesData.newNotesData.push_back(noteBar);
	}
}

nlohmann::json ScoreDataManager::Deserialize(const std::string& fullFilePath) {
	std::ifstream file;
	//ファイルを開ける
	file.open(fullFilePath);

	//読み込めないなら止める
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json data;
	file >> data;

	//値を返す
	return data;
}

std::vector<std::string> ScoreDataManager::GetFolderName(const std::filesystem::path& path){
	//記録
	std::vector<std::string>folderName;
	//指定したパスにあるものがフォルダであれば格納
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			folderName.push_back(entry.path().filename().string());
		}
	}

	return folderName;
}
