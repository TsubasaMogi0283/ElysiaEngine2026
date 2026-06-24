#include "EndMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <MainScene/MainScene.h>
#include <Camera.h>
#include <DirectionalLight.h>

EndMainScene::EndMainScene() {
	input_ = Elysia::Input::GetInstance();
}

void EndMainScene::Initialize(){
	
}

void EndMainScene::Update(MainScene* mainScene){
#ifdef _DEBUG
	ImGui::Begin("EndScene");
	ImGui::End();

	mainScene;

#endif // _DEBUG
}

void EndMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	baseLight;
	camera;
}

void EndMainScene::PreDrawPostEffect(){

}

void EndMainScene::DrawPostEffect(){

}

void EndMainScene::DrawSprite(){

}
