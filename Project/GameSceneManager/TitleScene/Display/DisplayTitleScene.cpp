#include "DisplayTitleScene.h"

#include <TitleScene/TitleScene.h>
#include <TitleScene/End/EndTitleScene.h>

void DisplayTitleScene::Initialize(){

}

void DisplayTitleScene::Update(){
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
