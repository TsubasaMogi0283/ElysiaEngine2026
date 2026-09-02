#include "StartMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Easing.h>
#include <MainScene/MainScene.h>
#include <MainScene/Play/PlayMainScene.h>

StartMainScene::StartMainScene() {
	//インスタンスの取得
	input_ = Elysia::Input::GetInstance();
}

void StartMainScene::Initialize() {
	//メインシーンの空チェック
	assert(mainScene_);
}

void StartMainScene::Update() {

	float_t gaugePositionY = 0.0f;
	float_t scorePositionY = 0.0f;
	float_t easedT = 0.0f;
	//状態遷移
	//ローカル変数の宣言がswitchの中でできないの腹立つので関数ポインタでやっていきたい。
	switch (currentState_) {
	case StartMainSceneState::Transition:
		//トランジションから始まる
		currentState_ = StartMainSceneState::UIMove;
		break;

	case StartMainSceneState::UIMove:

		//線形補間の時間を加算
		startMoveT_ += DELTA_TIME_ * 2.0f;
		startMoveT_ = std::clamp(startMoveT_, 0.0f, 1.0f);
		//イージング
		//種類はそろえた方が統一感が出るのでEaseInOutQuadに統一する
		easedT = Easing::EaseInOutQuad(startMoveT_);

		//ゲージ
		gaugePositionY = SingleCalculation::Lerp(mainScene_->GetInitialGaugePosition().y, mainScene_->GetGaugeDisplayPosition().y, easedT);
		mainScene_->SetGaugePosition({ mainScene_->GetGaugeDisplayPosition().x, gaugePositionY });

		//スコア
		scorePositionY = SingleCalculation::Lerp(mainScene_->GetInitialScorePositionY(), mainScene_->GetScoreDisplayPositionY(), easedT);
		mainScene_->SetScorePositionsY(scorePositionY);

		break;

	case StartMainSceneState::ReadyGo:
		//Ready?&Go!!の表示
		break;

	case StartMainSceneState::ToPlayScene:
		//プレイシーンへ

		break;
	}

#ifdef _DEBUG
	ImGui::Begin("メインシーン(開始)");
	ImGui::InputFloat("開始線形補間の値", &startMoveT_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<PlayMainScene>());
		return;
	}

#endif // _DEBUG
}

void StartMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight) {
	camera;
	baseLight;
}

void StartMainScene::DrawSprite() {

}
