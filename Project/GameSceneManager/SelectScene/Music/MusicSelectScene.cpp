#include "MusicSelectScene.h"

#include <numbers>

#include <SelectScene/SelectScene.h>
#include <GameSceneManager.h>
#include <Input.h>
#include <TextureManager.h>
#include <WindowsSetup.h>
#include <MiniAudio.h>

#include <SelectScene/Difficulty/DifficultySelectScene.h>

MusicSelectScene::MusicSelectScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//オーディオ
	miniAudio_ = Elysia::MiniAudio::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void MusicSelectScene::Initialize(){

	//全ての楽曲データを取得
	std::vector<ExistMusicScoreData> existData = selectScene_->GetExistMusicScoreData();
	for (size_t i = 0u;i < existData.size();i++) {
		jacketTextureHandleVector.push_back(existData[i].jacketTextureHandle);
	}

	//フレーム
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Jacket/DifficultyFrame.png");
	Vector2<float_t>frameTextureSize = textureManager_->GetTextureSize(frameTextureHandle);
	
#pragma region 矢印
	//テクスチャの読み込み
	uint32_t arrowTextureHandle = textureManager_->Load("Resources/Sprite/Select/Arrow.png");
	//サイズの取得
	Vector2<float_t>arrowTextureSize = textureManager_->GetTextureSize(arrowTextureHandle);
	
	//左右の座標を設定
	arrowArray_[Arrow::Direction::Left].position = {
			.x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 2u) - (arrowTextureSize.x / 2.0f + frameTextureSize.x / 4.0f),
			.y = 360.0f
	};
	arrowArray_[Arrow::Direction::Right].position = {
			.x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 2u) + arrowTextureSize.x / 2.0f + frameTextureSize.x / 4.0f,
			.y = 360.0f
	};
	for (uint8_t i = 0u; i < ARRAOW_AMOUNT_; i++) {
		arrowArray_[i].sprite = Elysia::Sprite::Create(arrowTextureHandle);
		arrowArray_[i].sprite->SetPosition(arrowArray_[i].position);
		arrowArray_[i].sprite->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
		//右は反転
		if (i == 1) {
			arrowArray_[i].sprite->SetRotate(std::numbers::pi_v<float_t>);
		}
	}
#pragma endregion

	//選択の効果音の読み込み
	selectSEHandle_ = miniAudio_->Load("Resources/Audio/SE/Select.wav");
}

void MusicSelectScene::Update(){
	//各状態を実行
	(this->*functionTable[static_cast<size_t>(currentState_)])();

}


void MusicSelectScene::DrawSprite(){
	//矢印
	for (uint8_t i = 0u; i < ARRAOW_AMOUNT_; i++) {
		arrowArray_[i].sprite->Draw();
	}
}

void MusicSelectScene::OpenTransition(){
	//トランジション開ける
	if (selectScene_->GetGameSceneManager()->GetTransition()->SetOpenTransition()) {
		currentState_ = MusicSelectSceneState::MusicSelect;
	}
}

void MusicSelectScene::MusicSelect(){
	//楽曲
	//右に選択
	if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
		miniAudio_->Play(selectSEHandle_, false);
	}
	//左に選択
	else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
		miniAudio_->Play(selectSEHandle_, false);
	}

	// 選択曲が変わったときだけ再生し直す
	if (currentSelectedMusicNumber_ != playingMusicNumber_) {
		// 前の曲を止める
		if (playingMusicNumber_ >= 0) {
			miniAudio_->Stop(selectScene_->GetExistMusicScoreData()[playingMusicNumber_].musicHandle);
		}

		float_t startTime = 0.0f;
		float_t endTime = 2.0f;
		//NeonStreet
		if (currentSelectedMusicNumber_ == 1) {
			startTime = 70.3f;
			endTime = 96.0f;
		} else {
			startTime = 48.5f;
			endTime = 61.0f;
		}
		miniAudio_->PartlyLoop(selectScene_->GetExistMusicScoreData()[currentSelectedMusicNumber_].musicHandle, startTime, endTime);
		playingMusicNumber_ = currentSelectedMusicNumber_;
	}

	//決定
	if (input_->IsTriggerKey(DIK_RETURN)) {
		currentState_ = MusicSelectSceneState::Decide;
	}

}

void MusicSelectScene::Decide(){
	//難易度選択へ
	selectScene_->ChangeSelectScene(std::make_unique<DifficultySelectScene>());
}
