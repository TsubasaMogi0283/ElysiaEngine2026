#include "DisplayTitleScene.h"

#include <TextureManager.h>
#include <Input.h>
#include <TitleScene/TitleScene.h>
#include <GameSceneManager.h>
#include <TitleScene/End/EndTitleScene.h>
#include <Audio.h>

DisplayTitleScene::DisplayTitleScene(){
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
}

void DisplayTitleScene::Initialize(){
	//スペースでスタート
	uint32_t startTextureHandle = textureManager_->Load("Resources/Sprite/Title/StartText.png");
	startTextSprite_ = Elysia::Sprite::Create(startTextureHandle);
	startTextSprite_->SetInvisible(true);
	decideSEHandle_ = audio_->Load("Resources/Audio/SE/Deside.wav");
	selectSEHandle_ = audio_->Load("Resources/Audio/SE/Select.wav");
	titleScene_->StartBGM();
}

void DisplayTitleScene::Update(){
	//各状態の更新
	(this->*functionTable[static_cast<size_t>(currentState_)])();

	
}


void DisplayTitleScene::DrawSprite(){
	//スペースでスタート
	startTextSprite_->Draw();
}

void DisplayTitleScene::StartSelect(){

	//点滅
	flashTime_ += DELTA_TIME_;
	//非表示
	if (flashTime_ <= FLASH_INTERVAL_) {
		startTextSprite_->SetInvisible(true);
	}
	//表示
	else if (flashTime_ <= FLASH_INTERVAL_ * 2.0f) {
		startTextSprite_->SetInvisible(false);
	}
	else {
		flashTime_ = 0.0f;
	}

	//右で開始を選択
	if (input_->IsTriggerKey(DIK_RIGHT) || input_->IsTriggerKey(DIK_D)) {
		titleScene_->SetSelectStart();
		audio_->Play(selectSEHandle_, false);
	}

	//左で終了を選択
	if (input_->IsTriggerKey(DIK_LEFT) || input_->IsTriggerKey(DIK_A)) {
		titleScene_->SetSelectEnd();
		audio_->Play(selectSEHandle_, false);
	}


	//SPACEを押したらスタート演出へ
	if (input_->IsTriggerKey(DIK_SPACE)) {
		audio_->Play(decideSEHandle_, false);
		currentState_= DisplayTitleSceneState::Decide;

	}

}

void DisplayTitleScene::Decide(){

	//点滅
	rapidFlashTime_ += DELTA_TIME_;
	//非表示
	if (rapidFlashTime_ <= RAPID_FLASH_TIME_) {
		startTextSprite_->SetInvisible(true);
	}
	//表示
	else if(rapidFlashTime_ <= RAPID_FLASH_TIME_ *2.0f){
		startTextSprite_->SetInvisible(false);
	}
	else {
		rapidFlashTime_ = 0.0f;
		rapidFlashCount_++;
	}
	


	//指定回数高速点滅したらセレクトシーンへ行く演出
	if (rapidFlashCount_ >= MAX_RAPID_FLASH_COUNT_) {
		currentState_ = DisplayTitleSceneState::ToSelect;
	}
}

void DisplayTitleScene::ToSelect(){
	
	//表示タイトルシーン
	if (true) {
		titleScene_->ChangeTitleScene(std::make_unique<EndTitleScene>());
	}
}

