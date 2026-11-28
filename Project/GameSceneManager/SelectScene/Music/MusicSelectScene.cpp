#include "MusicSelectScene.h"

#include <numbers>
#include <algorithm>

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
	//選択時の位置はウィンドウの中心にする
	selectedJacketPositionX_ = static_cast<float_t>(windowsSetup_->GetClientSize().x/2u);

	//全ての楽曲データを取得
	existMusicScoreDataVector_ = selectScene_->GetExistMusicScoreData();
	for (size_t i = 0u;i < existMusicScoreDataVector_.size();i++) {

		//難易度が1つでも格納されているかチェック
		bool isExistAnyDifficulty= std::any_of(existMusicScoreDataVector_[i].isExistDifficultyArray.begin(), existMusicScoreDataVector_[i].isExistDifficultyArray.end(), [](const bool& isExist) {
			return isExist;
			});

		
		//空では無ければ格納していく
		if (existMusicScoreDataVector_[i].musicHandle != -1&& isExistAnyDifficulty){
			//情報を設定していく
			MusicJacket musicJacket = {
				//ハンドル
				.mainTextureHandle = existMusicScoreDataVector_[i].jacketTextureHandle,
				//格納順の番号
				.index = static_cast<uint8_t>(i),
				//選択されているかどうか
				.isSelected = false,
				//座標
				.position = {},
				//ジャケットメイン部分
				.mainSprite = Elysia::Sprite::Create(existMusicScoreDataVector_[i].jacketTextureHandle),
			};
			
			//挿入
			musicJacketVector_.push_back(std::move(musicJacket));
		}	
	}


#ifdef _DEBUG
	currentSelectedMusicNumber_ = 1;
#endif // _DEBUG


	for (uint8_t i = 0u; i < musicJacketVector_.size(); i++) {

		//現在選択中の楽曲からの番号差分
		int32_t musicDifference = musicJacketVector_[i].index - currentSelectedMusicNumber_;
		Vector2<float_t>position = {
			.x = static_cast<float_t>(musicDifference) * JACKET_INTERVAL_X_ + selectedJacketPositionX_ ,
			.y = JACKET_INITIAL_POSITION_Y_ };
		musicJacketVector_[i].mainSprite->SetPosition(position);


		//アンカーポイントの設定
		musicJacketVector_[i].mainSprite->SetAnchorPoint({ .x = 0.5f,.y = 1.0f });

		//選択されている時
		if (currentSelectedMusicNumber_ == musicJacketVector_[i].index) {
			musicJacketVector_[i].isSelected = true;
			musicJacketVector_[i].mainSprite->SetScale({ .x = SELECTED_SCALE_,.y = SELECTED_SCALE_ });
		}
		else {
			musicJacketVector_[i].isSelected = false;
			musicJacketVector_[i].mainSprite->SetScale({ .x = NO_SELECTED_SCALE_,.y = NO_SELECTED_SCALE_ });
		}
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


	for (uint8_t i = 0u; i < musicJacketVector_.size(); i++) {

		//現在選択中の楽曲からの番号差分
		int32_t musicDifference=musicJacketVector_[i].index - currentSelectedMusicNumber_;

		//座標の設定
		Vector2<float_t> position = {
			.x = static_cast<float_t>(musicDifference) * JACKET_INTERVAL_X_ + selectedJacketPositionX_ ,
			.y = JACKET_INITIAL_POSITION_Y_ 
		};
		musicJacketVector_[i].mainSprite->SetPosition(position);
	}
}


void MusicSelectScene::DrawSprite(){
	//矢印
	for (uint8_t i = 0u; i < ARRAOW_AMOUNT_; i++) {
		arrowArray_[i].sprite->Draw();
	}

	//ジャケットの描画
	for (const auto& musicJacketSprite : musicJacketVector_) {
		musicJacketSprite.mainSprite->Draw();
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
		currentSelectedMusicNumber_++;
		miniAudio_->Play(selectSEHandle_, false);
	}
	//左に選択
	else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
		currentSelectedMusicNumber_--;
		miniAudio_->Play(selectSEHandle_, false);
	}

	//範囲制限
	currentSelectedMusicNumber_ = std::clamp(currentSelectedMusicNumber_, 0, static_cast<int32_t>(existMusicScoreDataVector_.size()-1));

	for (uint8_t i = 0u; i < musicJacketVector_.size(); i++) {
		//選択されている時
		if (currentSelectedMusicNumber_ == musicJacketVector_[i].index) {
			musicJacketVector_[i].isSelected = true;
			musicJacketVector_[i].mainSprite->SetScale({ .x = SELECTED_SCALE_,.y = SELECTED_SCALE_ });
		}
		else {
			musicJacketVector_[i].isSelected = false;
			musicJacketVector_[i].mainSprite->SetScale({ .x = NO_SELECTED_SCALE_,.y = NO_SELECTED_SCALE_ });
		}
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

	//タイトルへ戻る
	if (input_->IsTriggerKey(DIK_ESCAPE)) {
		miniAudio_->Stop(selectScene_->GetExistMusicScoreData()[currentSelectedMusicNumber_].musicHandle);
		currentState_ = MusicSelectSceneState::Return;
	}

}

void MusicSelectScene::Decide(){
	//難易度選択へ
	selectScene_->ChangeSelectScene(std::make_unique<DifficultySelectScene>());
}

void MusicSelectScene::Return(){
	//タイトルに戻る設定
	selectScene_->SetIsToTitle();
}
