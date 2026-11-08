#include "SelectScene.h"

#include <imgui.h>
#include <numbers>
#include <algorithm>

#include "Input.h"
#include "ModelManager.h"
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <Audio.h>
#include <WindowsSetup.h>
#include <StringOption.h>

SelectScene::SelectScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void SelectScene::Initialize(){
	
	//カメラ
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 5.0f,.z = -10.0f };

	//平行光源
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//ジャケット
	jacket_ = std::make_unique<Jacket>();
	
	//テクスチャの読みこみ
	std::vector<uint32_t>jacketTextureHandleVector = {};
	
	//全ての楽曲データを取得
	allScoreData_ = gameSceneManager_->GetScoreDataManager()->GetExistScoreData();

	std::vector<ExistMusicScoreData> existData = allScoreData_;
	for (size_t i = 0u;i < existData.size();i++) {
		jacketTextureHandleVector.push_back(existData[i].jacketTextureHandle);
	}
	//フレーム
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Jacket/DifficultyFrame.png");
	Vector2<int32_t>frameTextureSize = { 
		.x = static_cast<int32_t>(textureManager_->GetTextureWidth(frameTextureHandle)),
		.y = static_cast<int32_t>(textureManager_->GetTextureHeight(frameTextureHandle))
	};
	//初期化
	jacket_->Initialize(jacketTextureHandleVector, frameTextureHandle);
	jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));

	uint32_t arrowTextureHandle = textureManager_->Load("Resources/Sprite/Select/Arrow.png");
	Vector2<float_t>arrowTextureSize = {
		.x = static_cast<float_t>(textureManager_->GetTextureWidth(arrowTextureHandle)),
		.y = static_cast<float_t>(textureManager_->GetTextureHeight(arrowTextureHandle))
	};

	arrowPositionArray_[0] = {
			.x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 2u) - arrowTextureSize.x / 2.0f - frameTextureSize.x / 4.0f,
			.y = 360.0f
	};
	arrowPositionArray_[1] = {
			.x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 2u) + arrowTextureSize.x / 2.0f+ frameTextureSize .x/4.0f,
			.y = 360.0f
	};
	for (uint8_t i = 0u; i < ARRAOW_AMOUNT_; i++) {
		arrowSpriteArray_[i] = Elysia::Sprite::Create(arrowTextureHandle);
		arrowSpriteArray_[i]->SetPosition(arrowPositionArray_[i]);
		arrowSpriteArray_[i]->SetAnchorPoint({.x=0.5f,.y=0.5f});
		//右は反転
		if (i == 1) {
			arrowSpriteArray_[i]->SetRotate(std::numbers::pi_v<float_t>);
		}
	}
	
	//決定
	uint32_t decideTexturehandle = textureManager_->Load("Resources/Sprite/Select/Decide.png");
	decideSprite_ = Elysia::Sprite::Create(decideTexturehandle);
	decideSprite_->SetScale({ .x = 0.8f,.y = 0.8f });
	decideSprite_->SetPosition({ .x = 1050,.y = 500 });
	//戻る
	uint32_t backTexturehandle = textureManager_->Load("Resources/Sprite/Select/Back.png");
	backSprite_ = Elysia::Sprite::Create(backTexturehandle);
	backSprite_->SetScale({ .x = 0.8f,.y = 0.8f });
	backSprite_->SetPosition({ .x = 10,.y = 500 });


	decideSEHandle_ = audio_->Load("Resources/Audio/SE/Deside.wav");
	selectSEHandle_ = audio_->Load("Resources/Audio/SE/Select.wav");
}

void SelectScene::Update(){

	if (gameSceneManager_->GetTransition()->SetOpenTransition()) {
		isOpendTransiont_ = true;

		float startTime = 0.0f;
		float endTime = 2.0f;
		if (currentSelectedMusicNumber_ == 1) {
			startTime = 49.5f;
			endTime = 62.0f;
		} else {
			startTime = 48.5f;
			endTime = 61.0f;
		}
		audio_->PartlyLoopPlayMp3(allScoreData_[currentSelectedMusicNumber_].musicHandle, startTime, endTime);

	}
	if (isOpendTransiont_) {

		//楽曲
		//右に選択
		if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
			jacket_->SetIsMoveToRight(true);
			audio_->Play(selectSEHandle_,false);
			
		}
		//左に選択
		else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
			jacket_->SetIsMoveToLeft(true);
			audio_->Play(selectSEHandle_, false);
		}

		//難易度の設定
		//上を選択
		if (input_->IsTriggerKey(DIK_W) || input_->IsTriggerKey(DIK_UP)) {
			currentDifficultyNumber_++;
			audio_->Play(selectSEHandle_, false);
		}
		//下を選択
		if (input_->IsTriggerKey(DIK_S) || input_->IsTriggerKey(DIK_DOWN)) {
			currentDifficultyNumber_--;
			audio_->Play(selectSEHandle_, false);
		}
		//簡単から難しいまでにする
		currentDifficultyNumber_ = std::clamp(currentDifficultyNumber_, static_cast<int8_t>(Difficulty::Easy), static_cast<int8_t>(Difficulty::Hard));
		jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));

		//決定
		if (input_->IsTriggerKey(DIK_RETURN)) {
			jacket_->SetIsDecide();
			//選択した楽曲のデータを取得
			gameSceneManager_->SetTemporaryMusicScoreData(allScoreData_[currentSelectedMusicNumber_]);
			gameSceneManager_->SetTemporaryDifficulty(currentDifficultyNumber_);
			gameSceneManager_->SetTemporaryHandle(allScoreData_[currentSelectedMusicNumber_].musicHandle, allScoreData_[currentSelectedMusicNumber_].jacketTextureHandle);
			audio_->Play(decideSEHandle_, false);
		}

		currentSelectedMusicNumber_ = jacket_->GetCurrentSelectMusicNumber();

		
		

		// 選択曲が変わったときだけ再生し直す
		if (currentSelectedMusicNumber_ != playingMusicNumber_) {
			// 前の曲を止める（以前作ったReset等）
			if (playingMusicNumber_ >= 0) {
				audio_->Reset(allScoreData_[playingMusicNumber_].musicHandle);
			}

			float startTime = 0.0f;
			float endTime = 2.0f;
			//NeonStreet
			if (currentSelectedMusicNumber_ == 1) {
				startTime = 70.3f;
				endTime = 96.0f;
			}
			else {
				startTime = 48.5f;
				endTime = 61.0f;
			}
			audio_->PartlyLoopPlayMp3(allScoreData_[currentSelectedMusicNumber_].musicHandle, startTime, endTime);
			playingMusicNumber_ = currentSelectedMusicNumber_;
		}
		
		//処理終了
		if (jacket_->GetIsProcessEnd()) {
			//待機時間
			waitingTime_ += DELTA_TIME_;
		}
	}

	//ジャケット
	jacket_->Update();
	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();
	

	//指定時間を超えたらビネット
	//仮置きでトランジェント
	if (waitingTime_ >= WATING_TIME_) {
		if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
			isEnd_ = true;
		}
	}

	//メインシーン
	if (isEnd_) {
		
		for (uint8_t i = 0u;i < allScoreData_.size();i++) {
			audio_->Reset(allScoreData_[i].musicHandle);
		}

		gameSceneManager_->ChangeScene("Main");
		return;
	}

#ifdef _DEBUG
	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG

}

void SelectScene::DrawObject3D(){
	
}

void SelectScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void SelectScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void SelectScene::DrawSprite(){
	//ジャケット
	jacket_->DrawSprite();

	for (uint8_t i = 0u; i < ARRAOW_AMOUNT_; i++) {
		arrowSpriteArray_[i]->Draw();
	}

	//決定
	decideSprite_->Draw();
	//戻る
	backSprite_->Draw();

}


