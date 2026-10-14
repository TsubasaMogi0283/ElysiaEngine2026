#include "DisplayTitleScene.h"

#include <TextureManager.h>
#include <Input.h>
#include <TitleScene/TitleScene.h>
#include <GameSceneManager.h>
#include <TitleScene/End/EndTitleScene.h>

DisplayTitleScene::DisplayTitleScene(){
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力
	input_ = Elysia::Input::GetInstance();
}

void DisplayTitleScene::Initialize(){
	//スペースでスタート
	uint32_t startTextureHandle = textureManager_->Load("Resources/Sprite/Title/StartText.png");
	startTextSprite_ = Elysia::Sprite::Create(startTextureHandle);


}

void DisplayTitleScene::Update(){
	//各状態の更新
	(this->*functionTable[static_cast<size_t>(currentState_)])();

	if (false) {
		titleScene_->ChangeTitleScene(std::make_unique<EndTitleScene>());
	}
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


	//SPACEを押したらスタート演出へ
	if (input_->IsTriggerKey(DIK_SPACE)) {
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
		titleScene_->ChangeTitleScene(std::make_unique<DisplayTitleScene>());
	}
}

