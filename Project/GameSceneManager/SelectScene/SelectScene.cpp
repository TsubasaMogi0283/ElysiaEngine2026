#include "SelectScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include <GameSceneManager.h>

SelectScene::SelectScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
}

void SelectScene::Initialize(){
	
	//カメラ
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 5.0f,.z = -10.0f };

	//平行光源
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//ジャケット
	jacket_ = std::make_unique<Jacket>();

}

void SelectScene::Update(){

	//右に選択
	if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
		jacket_->SetIsMoveToRight(true);
	}
	//左に選択
	else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
		jacket_->SetIsMoveToLeft(true);
	}

	//ジャケット
	jacket_->Update();

	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();

	//処理終了
	if (jacket_->GetIsProcessEnd()) {
		//待機時間
		waitingTime_ += DELTA_TIME_;
	}

	//指定時間を超えたらビネット
	//仮置きでトランジェント
	if (waitingTime_ >= WATING_TIME_) {
		if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
			isEnd_ = true;
		}

	}

	//メインシーン
	if (isEnd_) {
		gameSceneManager_->ChangeScene("Main");
		return;
	}


#ifdef _DEBUG
	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG


}

void SelectScene::DrawObject3D(){
	
}

void SelectScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void SelectScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void SelectScene::DrawSprite(){
	//ジャケット
	jacket_->DrawSprite();
}


