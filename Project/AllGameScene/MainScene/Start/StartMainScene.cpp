#include "StartMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <MainScene/MainScene.h>
#include <MainScene/Play/PlayMainScene.h>

StartMainScene::StartMainScene(){
	//インスタンスの取得
	input_ = Elysia::Input::GetInstance();
}

void StartMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);
}

void StartMainScene::Update(){

#ifdef _DEBUG
	ImGui::Begin("StartScene");
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_N)) {
		mainScene_->ChangeMainScene(std::make_unique<PlayMainScene>());
	}

#endif // _DEBUG
}

void StartMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void StartMainScene::PreDrawPostEffect(){

}

void StartMainScene::DrawPostEffect(){

}

void StartMainScene::DrawSprite(){

}
