#include "MainScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "CollisionCalculation.h"
#include "PushBackCalculation.h"
#include <AnimationManager.h>
#include <MainScene/Start/StartMainScene.h>

MainScene::MainScene(){
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void MainScene::Initialize(){

	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("CollisionTest/CollisionTest.json");

	//カメラの初期化
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float_t> / 6.0f;
	camera_.translate = { .x = 0.0f,.y = 21.0f,.z = -40.0f };

	//平行光源の初期化
	directionalLight_.Initialize();
	spotLight.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();
	
	//メインシーンの中
	baseMainScene_ = std::make_unique<StartMainScene>();
	baseMainScene_->Initialize();
}

void MainScene::Update(Elysia::GameManager* gameManager){

	gameManager;
	levelDataManager_->Update(levelHandle_);

#ifdef _DEBUG
	ImGui::Begin("メインシーン");
	ImGui::End();
#endif // _DEBUG


	//更新
	baseMainScene_->Update(this);
	directionalLight_.Update();
	spotLight.Update();
	camera_.Update();
}

void MainScene::DrawObject3D(){
	//オブジェクトの描画
	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);
	baseMainScene_->DrawObject3D(camera_,directionalLight_);

}

void MainScene::PreDrawPostEffect(){
	
	//ポストエフェクト描画前処理
	baseMainScene_->PreDrawPostEffect();
	backTexture_->PreDraw();
}

void MainScene::DrawPostEffect(){
	//ポストエフェクト描画処理
	baseMainScene_->DrawPostEffect();
	backTexture_->Draw();
}

void MainScene::DrawSprite(){
	//スプライトの描画
	baseMainScene_->DrawSprite();
}


