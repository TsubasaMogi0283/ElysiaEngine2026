#include "EndMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <TextureManager.h>
#include <WindowsSetup.h>
#include <MainScene/MainScene.h>
#include <GameSceneManager.h>
#include <Easing.h>

EndMainScene::EndMainScene() {
	//インスタンスの取得
	//インプット
	input_ = Elysia::Input::GetInstance();
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
	//ウィンドウ
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void EndMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);

	//各結果のテクスチャを読み込む
	const std::string RESULT_TEXTURE_FOLDER_PASS = "Resources/Sprite/Result/";
	resultTextureHandles_[static_cast<size_t>(ResultTextureSelection::AllPerfect)]=textureManager_->Load(RESULT_TEXTURE_FOLDER_PASS+"Complete.png");
	resultTextureHandles_[static_cast<size_t>(ResultTextureSelection::FullCombo)] = textureManager_->Load(RESULT_TEXTURE_FOLDER_PASS + "FullCombo.png");
	resultTextureHandles_[static_cast<size_t>(ResultTextureSelection::Complete)] = textureManager_->Load(RESULT_TEXTURE_FOLDER_PASS + "Complete.png");
	resultTextureHandles_[static_cast<size_t>(ResultTextureSelection::Failed)] = textureManager_->Load(RESULT_TEXTURE_FOLDER_PASS + "Failed.png");
	
	//生成
	resultSprite_ = Elysia::Sprite::Create(resultTextureHandles_[static_cast<size_t>(ResultTextureSelection::Complete)]	);
	//アンカーポイントの設定
	resultSprite_->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
	//スケールの設定
	resultSprite_->SetScale({ .x = RESULT_MAX_SIZE_, .y = RESULT_MAX_SIZE_ });
	//座標を設定
	Vector2<int32_t>initialPosition = {
		.x = windowsSetup_->GetClientSize().x/2u,
		.y = windowsSetup_->GetClientSize().y/2u
	};
	resultSprite_->SetPosition(initialPosition);
	//最初は非表示
	resultSprite_->SetInvisible(true);
}

void EndMainScene::Update(){

	//各状態の処理を実行
	(this->*functionTable[static_cast<int>(currentState_)])();

#ifdef _DEBUG
	ImGui::Begin("EndScene");
	ImGui::End();
#endif // _DEBUG
}

void EndMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	baseLight;
	camera;
}

void EndMainScene::DrawSprite(){
	//結果のスプライトの描画
	resultSprite_->Draw();
}

void EndMainScene::Wait(){
	//待機時間
	waitingTimeFormMain_ += DELTA_TIME_;
	if (waitingTimeFormMain_ > WAITING_TIME_) {
		currentState_ = EndMainSceneState::MoveResult;
	}
}

void EndMainScene::MoveResult(){
	//表示
	resultSprite_->SetInvisible(false);

	//縮小
	resultScaleT_ += DELTA_TIME_;
	float_t scaleDownT = SingleCalculation::InverseLerp(0.0f, RESULT_SCALE_DOWN_TIME_, resultScaleT_);
	scaleDownT = std::clamp(scaleDownT, 0.0f, 1.0f);
	float_t eased =Easing::EaseOutCubic(scaleDownT);
	float_t finalScale = SingleCalculation::Lerp(RESULT_MAX_SIZE_, RESULT_MIN_SIZE_, eased);
	resultSprite_->SetScale({.x= finalScale, .y= finalScale });

	//スケールが終わったら次へ
	if (scaleDownT >= 1.0f) {
		currentState_ = EndMainSceneState::DisplayResult;
	}

}

void EndMainScene::DisplayResult(){
	//指定した時間まで表示
	displayTime_ += DELTA_TIME_;
	if (displayTime_ >= MAX_DISPLAY_TIME_) {
		currentState_ = EndMainSceneState::Transition;
	}
}

void EndMainScene::Transition(){
	

	if (!isEndTransition_) {
		//トランジション
		if (mainScene_->GetGameSceneManager()->GetTransition()->SetCloseTransition()) {
			isEndTransition_ = true;
		}
	}
	else {
		readyNextTime_ += DELTA_TIME_;
		if (readyNextTime_ > READY_NEXT_TIME_) {
			//処理終了
			isEnd_ = true;
		}
	}
}
