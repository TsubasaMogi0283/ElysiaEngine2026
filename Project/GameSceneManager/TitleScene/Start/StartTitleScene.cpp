#include "StartTitleScene.h"
#include <TitleScene/TitleScene.h>
#include <TitleScene/Display/DisplayTitleScene.h>
#include <GameSceneManager.h>

void StartTitleScene::Initialize(){

}

void StartTitleScene::Update(){

	(this->*functionTable[static_cast<size_t>(currentState_)])();

	
	if (false) {
		titleScene_->ChangeMainScene(std::make_unique<DisplayTitleScene>());
	}
	
}

void StartTitleScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void StartTitleScene::DrawSprite(){

}

void StartTitleScene::OpenTransition(){

	//開いたら次の状態へ
	if (titleScene_->GetGameSceneManager()->GetTransition()->SetOpenTransition()) {
		isEndTransition_ = true;
	}

	if (isEndTransition_) {
		waitingTimeArray_[static_cast<size_t>(StartMainSceneState::OpenTransition)] += DELTA_TIME_;
		if (waitingTimeArray_[static_cast<size_t>(StartMainSceneState::OpenTransition)] >= NEXT_WAIT_TIME_) {
			//トランジションが終わったらUIの移動へ 
			currentState_ = StartMainSceneState::TextMove;
		}
	}
}

void StartTitleScene::TextMove(){
	//テキスト「DA・DA・PA!!」の移動演出



}

void StartTitleScene::ApperCharacter(){
	//キャラクターが走ってくる


}

void StartTitleScene::StartSelect()
{
}

void StartTitleScene::Decide()
{
}

void StartTitleScene::ToSelect()
{
}

void StartTitleScene::QuitGame()
{
}

void StartTitleScene::CloseTransition()
{
}
