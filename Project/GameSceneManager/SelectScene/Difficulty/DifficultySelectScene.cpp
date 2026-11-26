#include "DifficultySelectScene.h"

#include <SelectScene/SelectScene.h>
#include <GameSceneManager.h>
#include <Input.h>
#include <MiniAudio.h>

#include <Easing.h>
#include <Color.h>

DifficultySelectScene::DifficultySelectScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	miniAudio_ = Elysia::MiniAudio::GetInstance();
}

void DifficultySelectScene::Initialize(){

	//譜面の格納データを取得
	std::vector<ExistMusicScoreData> existData = selectScene_->GetExistMusicScoreData();

	//格納チェック
	//そこから表示する難易度を決める
	for (uint8_t i = 0u;i < static_cast<uint8_t>(Difficulty::Size);i++) {

		isDataExist_[i] = true;
	}

	//選択の効果音の読み込み
	selectSEHandle_ = miniAudio_->Load("Resources/Audio/SE/Select.wav");
}

void DifficultySelectScene::Update(){
	//int型に変換
	int8_t currentDifficultyNumber = static_cast<int8_t>(currentDifficultyNumber_);

	//難易度の設定
	//難しく
	if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
		currentDifficultyNumber++;
		miniAudio_->Play(selectSEHandle_, false);
	}
	//簡単に
	if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
		currentDifficultyNumber--;
		miniAudio_->Play(selectSEHandle_, false);
	}

	//範囲内にするようにする
	currentDifficultyNumber_ = static_cast<Difficulty>(currentDifficultyNumber);
	currentDifficultyNumber_ = std::clamp(currentDifficultyNumber_, Difficulty::Easy, Difficulty::Special);


	//難易度によって色を変える
	Vector4<float_t> frameColor = {};
	//難易度の色を設定
	switch (currentDifficultyNumber_) {
	case Difficulty::Easy:
		//簡単の色を設定
		frameColor = Color::Conevert(Color::EASY);
		break;

	case Difficulty::Normal:
		//普通の色を設定
		frameColor = Color::Conevert(Color::NORMAL);
		break;

	case Difficulty::Hard:
		//難しいの色を設定
		frameColor = Color::Conevert(Color::HARD);
		break;

	case Difficulty::Special:
		//特別の色を設定
		specialHsv_.hue += 0.25f * DELTA_TIME_;
		specialHsv_.hue -= std::floor(specialHsv_.hue);
		frameColor = Color::HsvToRgb(specialHsv_);
		break;
	}


	//決定
	if (input_->IsTriggerKey(DIK_RETURN)) {
		//jacket_->SetIsDecide();
		//選択した楽曲のデータを取得
		//selectScene_->GetGameSceneManager()->SetTemporaryMusicScoreData(allScoreData_[currentSelectedMusicNumber_]);
		//selectScene_->GetGameSceneManager()->SetTemporaryDifficulty(currentDifficultyNumber_);
		//selectScene_->GetGameSceneManager()->SetTemporaryHandle(allScoreData_[currentSelectedMusicNumber_].musicHandle, allScoreData_[currentSelectedMusicNumber_].jacketTextureHandle);
		miniAudio_->Play(decideSEHandle_, false);
	}
	
	//楽曲選択へ戻る
	if (input_->IsTriggerKey(DIK_ESCAPE)) {
		isReturn_ = true;
	}

	if (isReturn_) {
		selectScene_->SetIsEnd();
	}

}
