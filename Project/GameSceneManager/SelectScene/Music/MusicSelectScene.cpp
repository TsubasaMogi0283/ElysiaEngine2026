#include "MusicSelectScene.h"

#include <SelectScene/SelectScene.h>
#include <GameSceneManager.h>
#include <Input.h>
#include <TextureManager.h>
#include <SelectScene/Difficulty/DifficultySelectScene.h>

MusicSelectScene::MusicSelectScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void MusicSelectScene::Initialize(){

	//全ての楽曲データを取得
	std::vector<ExistMusicScoreData> existData = selectScene_->GetAllScoreData();
	for (size_t i = 0u;i < existData.size();i++) {
		jacketTextureHandleVector.push_back(existData[i].jacketTextureHandle);
	}
	//フレーム
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Jacket/DifficultyFrame.png");
	Vector2<float_t>frameTextureSize = textureManager_->GetTextureSize(frameTextureHandle);
	//初期化
	jacket_->Initialize(jacketTextureHandleVector, frameTextureHandle);
	jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));


}

void MusicSelectScene::Update(){

	//トランジション開ける
	if (selectScene_->GetGameSceneManager()->GetTransition()->SetOpenTransition()) {
		isOpenTransition_ = true;
	}

	if (isOpenTransition_) {
		//楽曲
		//右に選択
		if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
			jacket_->SetIsMoveToRight(true);
			miniAudio_->Play(selectSEHandle_, false);

		}
		//左に選択
		else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
			jacket_->SetIsMoveToLeft(true);
			miniAudio_->Play(selectSEHandle_, false);
		}





		// 選択曲が変わったときだけ再生し直す
		if (currentSelectedMusicNumber_ != playingMusicNumber_) {
			// 前の曲を止める
			if (playingMusicNumber_ >= 0) {
				miniAudio_->Stop(allScoreData_[playingMusicNumber_].musicHandle);
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
			miniAudio_->PartlyLoop(allScoreData_[currentSelectedMusicNumber_].musicHandle, startTime, endTime);
			playingMusicNumber_ = currentSelectedMusicNumber_;
		}
	}

	


	if (false) {
		//難易度選択へ
		selectScene_->ChangeSelectScene(std::make_unique<DifficultySelectScene>());
	}

}

void MusicSelectScene::DrawObject(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void MusicSelectScene::DrawSprite(){

}
