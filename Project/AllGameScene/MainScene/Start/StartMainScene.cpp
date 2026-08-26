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

	//線形補間の時間を加算
	startMoveT_ += DELTA_TIME_;
	float_t g = std::clamp(startMoveT_, 0.0f, 1.0f);

	//トランジションから始まる


	//UIの移動

	//Ready?&Go!!

	//プレイシーンへ

#ifdef _DEBUG
	ImGui::Begin("StartScene");
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
