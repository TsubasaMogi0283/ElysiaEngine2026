#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <MainScene/MainScene.h>
#include <MainScene/End/EndMainScene.h>

PlayMainScene::PlayMainScene(){
	input_ = Elysia::Input::GetInstance();
}

void PlayMainScene::Initialize(){
	
}

void PlayMainScene::Update(MainScene* mainScene){
#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_N)) {
		mainScene->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#endif // _DEBUG
}

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void PlayMainScene::PreDrawPostEffect(){

}

void PlayMainScene::DrawPostEffect(){

}

void PlayMainScene::DrawSprite(){

}
