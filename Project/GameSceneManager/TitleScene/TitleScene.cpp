#include "TitleScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "CollisionCalculation.h"
#include "PushBackCalculation.h"
#include <AnimationManager.h>
#include <GameSceneManager.h>
#include <TitleScene/Start/StartTitleScene.h>

TitleScene::TitleScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
}

void TitleScene::Initialize(){
	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("CollisionTest/CollisionTest.json");

	//カメラ
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float>/6.0f;
	camera_.translate = { .x = 0.0f,.y = 21.0f,.z = -40.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//各シーン
	baseTitleScene_ = std::make_unique<StartTitleScene>();
	baseTitleScene_->SetTitleScene(this);
	baseTitleScene_->Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();
}

void TitleScene::Update(){

	//各シーンの更新
	baseTitleScene_->Update();

	//レベルエディタの更新
	levelDataManager_->Update(levelHandle_);
	std::vector<AABB> objects = levelDataManager_->GetObjectAABBs(levelHandle_,"Stage");

	camera_.Update();
	directionalLight_.Update();

	//処理が終わったら次のシーンへ
	if (baseTitleScene_->GetIsEnd()) {
		gameManager_->ChangeScene("Select");
	}

}

void TitleScene::DrawObject3D(){
	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);
	//各シーン
	baseTitleScene_->DrawObject3D(camera_, directionalLight_);
}

void TitleScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void TitleScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void TitleScene::DrawSprite(){
	//各シーン
	baseTitleScene_->DrawSprite();
}


