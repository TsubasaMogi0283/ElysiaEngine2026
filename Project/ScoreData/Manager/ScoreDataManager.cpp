#include "ScoreDataManager.h"

#include <fstream>
#include <magic_enum/magic_enum.hpp>
#include <Audio.h>
#include <ScoreData/MusicScoreData.h>
#include <StringOption.h>

ScoreDataManager::ScoreDataManager(){
	//オーディオクラス
	audio_ = Elysia::Audio::GetInstance();
}

void ScoreDataManager::Initialize(){

	///フォルダの名前を取得
	std::vector<std::string>musicNameVector= GetFolderName(MUSIC_SCORE_PATH_);
	//難易度の名前を取得
	std::vector<std::string>difficultyNameVector ={};

	//難易度
	for (uint8_t i = 0u;i < static_cast<uint8_t>(Difficulty::Size);i++) {
		//名前を取り出す
		std::string_view difficulty = magic_enum::enum_name(static_cast<Difficulty>(i));
		//std::stringに変換
		std::string convertDifficulty(difficulty);
		//挿入
		difficultyNameVector.push_back(convertDifficulty);
	}


	//基本的には譜面データはJSONでやる
	Load("HighwaySunset/HighwaySunset_Easy.json");

	//Construction
	Load("Construction/Construction_Easy.json");
	Load("Construction/Construction_Normal.json");
	Load("Construction/Construction_Hard.json");
	Load("Construction/Construction_Special.json");
}

void ScoreDataManager::Load(const std::string& path) {
	//パスの結合
	std::string fullFilePath = MUSIC_SCORE_PATH_ + "/" + path;



	//JSON文字列から解凍したデータ
	nlohmann::json deserialized = Deserialize(fullFilePath);
	
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
	const std::string ID = TITLE + "_" + LEVEL;
	//楽曲ファイル名
	const std::string MUSIC_FILE_NAME = deserialized["filename"].get<std::string>();
	//拡張子を取得
	std::string MUSIC_FULL_EXTENSION = StringOption::FindExtension(MUSIC_SCORE_PATH_+"/" + MUSIC_FILE_NAME, MUSIC_FILE_NAME);
	//楽曲ファイルパス
	std::string MUSIC_FULL_FILE_PATH = MUSIC_SCORE_PATH_ + "/" + MUSIC_FILE_NAME +"/" + MUSIC_FILE_NAME + MUSIC_FULL_EXTENSION;

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
	musicNotesData.fullFilePath_ = fullFilePath;
	musicNotesData.musicHandle = audio_->Load(MUSIC_FULL_FILE_PATH);
	//ノーツの配置
	Place(musicNotesData, deserialized);
	//挿入
	musicInformation.insert(make_pair(MUSIC_INFORMATION.id,musicNotesData));

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
