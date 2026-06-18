#include "EndMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <MainScene/MainScene.h>

void EndMainScene::Initialize(){
	
}

void EndMainScene::Update(MainScene* mainScene){
#ifdef _DEBUG
	ImGui::Begin("EndScene");
	ImGui::End();

	mainScene;

#endif // _DEBUG
}

void EndMainScene::DrawObject3D(const BaseLight& baseLight){
	baseLight;
}

void EndMainScene::PreDrawPostEffect(){

}

void EndMainScene::DrawPostEffect(){

}

void EndMainScene::DrawSprite(){

}
