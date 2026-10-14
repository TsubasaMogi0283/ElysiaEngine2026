#include "StartTitleScene.h"
#include <TitleScene/TitleScene.h>
#include <TitleScene/Display/DisplayTitleScene.h>
#include <GameSceneManager.h>

void StartTitleScene::Initialize(){

}

void StartTitleScene::Update(){
	//各状態の更新
	(this->*functionTable[static_cast<size_t>(currentState_)])();
}


void StartTitleScene::OpenTransition(){

	//開いたら次の状態へ
	if (titleScene_->GetGameSceneManager()->GetTransition()->SetOpenTransition()) {
		isEndTransition_ = true;
	}

	if (isEndTransition_) {
		waitingTimeArray_[static_cast<size_t>(StartTitleSceneState::OpenTransition)] += DELTA_TIME_;
		if (waitingTimeArray_[static_cast<size_t>(StartTitleSceneState::OpenTransition)] >= NEXT_WAIT_TIME_) {
			//トランジションが終わったらUIの移動へ 
			currentState_ = StartTitleSceneState::TextMove;
		}
	}
}

void StartTitleScene::TextMove(){
	//テキスト「DA・DA・PA!!」の移動演出
	titleScene_->GetGameTextObject()->SetIsMove(true);
	//処理が終了したら次へ
	if (titleScene_->GetGameTextObject()->GetIsProcessEnd()) {
		currentState_ = StartTitleSceneState::ApperCharacter;
	}

}

void StartTitleScene::ApperCharacter(){
	//キャラクターが走ってくる
	//表示タイトルシーン
	if (true) {
		titleScene_->ChangeTitleScene(std::make_unique<DisplayTitleScene>());
	}

}