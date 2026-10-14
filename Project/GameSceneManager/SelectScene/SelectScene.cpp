#include "SelectScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "CollisionCalculation.h"
#include "PushBackCalculation.h"

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

	jacketVector_ = std::make_unique<Jacket>();


}

void SelectScene::Update(){
	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();

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

}


