#include "StartTitleScene.h"
#include <TitleScene/TitleScene.h>
#include <TitleScene/Display/DisplayTitleScene.h>
#include <GameSceneManager.h>
#include <Easing.h>

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
		isEndGameTitleTextMove_ = true;
		
	}

	//選択肢の透明度を設定
	if (isEndGameTitleTextMove_) {
		//透明度の設定
		transparencyTime_ += DELTA_TIME_;
		float_t t = SingleCalculation::InverseLerp(0.0f, MAX_TRANSPARENCY_TIME_, transparencyTime_);
		t = std::clamp(t, 0.0f, 1.0f);
		selectTransparency_ = Easing::EaseInQuart(t);
		titleScene_->SetSelectModelTransparency(selectTransparency_);
		if (t >= 1.0f) {
			currentState_ = StartTitleSceneState::ApperCharacter;
		}
	}

}

void StartTitleScene::ApperCharacter(){
	//キャラクターが走ってくる
	//表示タイトルシーン
	if (true) {
		//開始を選択
		titleScene_->SetSelectStart();
		//表示シーンへ
		titleScene_->ChangeTitleScene(std::make_unique<DisplayTitleScene>());
	}

}