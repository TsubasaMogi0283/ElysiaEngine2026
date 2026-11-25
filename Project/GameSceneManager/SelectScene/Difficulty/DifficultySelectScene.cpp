#include "DifficultySelectScene.h"

#include <SelectScene/SelectScene.h>
#include <GameSceneManager.h>
#include <Input.h>

#include <Easing.h>

DifficultySelectScene::DifficultySelectScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
}

void DifficultySelectScene::Initialize(){


}

void DifficultySelectScene::Update(){

	//難易度の設定
	//上を選択
	if (input_->IsTriggerKey(DIK_W) || input_->IsTriggerKey(DIK_UP)) {
		currentDifficultyNumber_++;
		miniAudio_->Play(selectSEHandle_, false);
	}
	//下を選択
	if (input_->IsTriggerKey(DIK_S) || input_->IsTriggerKey(DIK_DOWN)) {
		currentDifficultyNumber_--;
		miniAudio_->Play(selectSEHandle_, false);
	}
}
