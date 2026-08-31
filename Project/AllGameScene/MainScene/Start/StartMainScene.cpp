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

	switch (currentState_) {
	case StartMainSceneState::Transition:
		//トランジションから始まる
		currentState_ = StartMainSceneState::UIMove;
		break;

	case StartMainSceneState::UIMove:
		//線形補間の時間を加算
		startMoveT_ += DELTA_TIME_;
		startMoveT_ = std::clamp(startMoveT_, 0.0f, 1.0f);
		//線形保管&イージングで滑らかに
		float_t gaugePositionY = SingleCalculation::Lerp(mainScene_->GetInitialGaugePosition().y, mainScene_->GetGaugeDisplayPosition().y, startMoveT_);
		mainScene_->SetGaugePosition({ mainScene_->GetGaugeDisplayPosition().x, gaugePositionY });

		break;

	case StartMainSceneState::ReadyGo:
		//Ready?&Go!!の表示
		

	case StartMainSceneState::ToPlayScene:

		break;
	}
	
#ifdef _DEBUG
	ImGui::Begin("StartScene");
	ImGui::InputFloat("startMoveT_", &startMoveT_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<PlayMainScene>());
		return;
	}

#endif // _DEBUG
}

void StartMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void StartMainScene::DrawSprite(){

}
