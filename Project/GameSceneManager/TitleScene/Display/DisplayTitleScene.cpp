#include "DisplayTitleScene.h"

#include <TitleScene/TitleScene.h>
#include <TitleScene/End/EndTitleScene.h>

void DisplayTitleScene::Initialize(){

}

void DisplayTitleScene::Update(){
	//各状態の更新
	(this->*functionTable[static_cast<size_t>(currentState_)])();


	if (false) {
		titleScene_->ChangeMainScene(std::make_unique<EndTitleScene>());
	}
}

void DisplayTitleScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void DisplayTitleScene::DrawSprite(){

}

void DisplayTitleScene::StartSelect(){

}

void DisplayTitleScene::Decide()
{
}

void DisplayTitleScene::ToSelect()
{
}

void DisplayTitleScene::QuitGame()
{
}
