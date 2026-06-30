#include "ScoreDataManager.h"

#include <fstream>
#include <Audio.h>
#include <ScoreData/MusicScoreData.h>
#include <StringOption.h>


ScoreDataManager::ScoreDataManager(){
	//オーディオクラス
	audio_ = Elysia::Audio::GetInstance();
}

void ScoreDataManager::Initialize(){
	//ToDo
	//Resourceフォルダに楽曲と譜面データを入れる
	//フォルダに楽曲名を入れる。楽曲名を全て探索し、数を数え、各難易度の譜面データを記録していく
	
	//基本的には譜面データはJSONでやる
	Load("HighwaySunset/HighwaySunset_Easy.json");
	//Load("UnknownFunction");
	//Load("C0ns+ruct10n");
}

void ScoreDataManager::Load(const std::string& path) {
	//パスの結合
	std::string fullFilePath = MUSIC_SCORE_PATH_ + path;

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized = Deserialize(fullFilePath);
	//ファイルパスの分解
	std::string folderName = {};
	std::string fileName = {};
	size_t slashPosition = path.find('/');

	if (slashPosition != std::string::npos) {
		//0からslashPositionまで
		folderName = path.substr(0, slashPosition);
		//「/」から最後まで
		fileName = path.substr(slashPosition + 1);
	}

	//楽曲情報を記録
	//タイトル
	const std::string TITLE = deserialized["title"].get<std::string>();
	//作曲
	const std::string PRODUCE = deserialized["produce"].get<std::string>();
	//レベル
	const std::string LEVEL = deserialized["level"].get<std::string>();
	//難易度
	const std::string DIFFICULTY = deserialized["difficulty"].get<std::string>();
	//BPM
	const std::string BPM = deserialized["bpm"].get<std::string>();
	//オフセット
	const std::string OFFSET = deserialized["offset"].get<std::string>();
	//ID
	const std::string ID = deserialized["id"].get<std::string>();
	//楽曲ファイル名
	const std::string MUSIC_FILE_NAME = deserialized["filename"].get<std::string>();
	//拡張子を取得
	std::string MUSIC_FULL_EXTENSION = StringOption::FindExtension(MUSIC_SCORE_PATH_+MUSIC_FILE_NAME, MUSIC_FILE_NAME);
	//楽曲ファイルパス
	std::string MUSIC_FULL_FILE_PATH = MUSIC_SCORE_PATH_ + MUSIC_FILE_NAME +"/" + MUSIC_FILE_NAME + MUSIC_FULL_EXTENSION;

	//ハンドルとパスを記録
	MusicScoreData musicNotesData = {};
	const MusicInformation MUSIC_INFORMATION = {
		.title = TITLE,
		.produce = PRODUCE,
		.level = LEVEL,
		.difficulty = std::atoi(DIFFICULTY.c_str()),
		.bpm = static_cast<float_t>(std::atof(BPM.c_str())),
		.offset = static_cast<float_t>(std::atof(OFFSET.c_str())),
		.id = ID,
		.fileName = MUSIC_FILE_NAME,
	};
	//挿入
	musicNotesData.musicScoreData_ = MUSIC_INFORMATION;
	musicNotesData.fullFilePath_ = fullFilePath;
	musicNotesData.handle = audio_->Load(MUSIC_FULL_FILE_PATH);
	//ノーツの配置
	Place(musicNotesData, deserialized);
	//挿入
	musicScoreData_.insert(make_pair(MUSIC_INFORMATION.id,musicNotesData));

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
			if (!row.is_array() || row.size() < 2) {
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
