#include "StartTitleScene.h"
#include <TitleScene/TitleScene.h>
#include <TitleScene/Display/DisplayTitleScene.h>
#include <GameSceneManager.h>

void StartTitleScene::Initialize(){

}

void StartTitleScene::Update(){

	//開いたら次の状態へ
	if (titleScene_->GetGameSceneManager()->GetTransition()->SetOpenTransition()) {

	}

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
