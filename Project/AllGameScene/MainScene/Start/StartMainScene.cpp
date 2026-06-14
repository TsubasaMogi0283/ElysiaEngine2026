#include "StartMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <MainScene/MainScene.h>
#include <MainScene/Play/PlayMainScene.h>

void StartMainScene::Initialize(){
	//インスタンスの取得
	input_ = Elysia::Input::GetInstance();
}

void StartMainScene::Update(MainScene* mainScene){

#ifdef _DEBUG
	ImGui::Begin("StartScene");
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_N)) {
		mainScene->ChangeMainScene(std::make_unique<PlayMainScene>());
	}

#endif // _DEBUG
}

void StartMainScene::DrawObject3D(){

}

void StartMainScene::PreDrawPostEffect(){

}

void StartMainScene::DrawPostEffect(){

}

void StartMainScene::DrawSprite(){

}
