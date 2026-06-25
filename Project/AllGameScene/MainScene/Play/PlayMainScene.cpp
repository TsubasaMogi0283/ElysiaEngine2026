#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Audio.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <MainScene/End/EndMainScene.h>
#include <Note/NormalTap/NormalTapNote.h>
#include <Note/Long/LongNote.h>

PlayMainScene::PlayMainScene(){
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
}

void PlayMainScene::Initialize(){
	//メインシーンを設定しているか
	assert(mainScene_);
	//auto i = gameManager->GetScoreDataManager();
	//audio_ = gameManager->GetScoreDataManager()->Initialize();
}

void PlayMainScene::Update(){

	//流れ終わったら終了シーンへ
	if (isNoteFlowEnd_) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_N)) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
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

