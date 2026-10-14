#include "TitleScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
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
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
}

void TitleScene::Initialize(){
	
	//カメラ
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 5.0f,.z = -30.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//各シーン
	baseTitleScene_ = std::make_unique<StartTitleScene>();
	baseTitleScene_->SetTitleScene(this);
	baseTitleScene_->Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//地面
	uint32_t groundModelHandle = modelManager_->Load("Resources/Model/Ground", "Ground.obj");
	groundModel_ = Elysia::Model::Create(groundModelHandle);
	groundWorldTransform_.Initialize();
	groundMaterial_.Initialize();

	//ゲーム名のオブジェクト
	uint32_t dModelHandle = modelManager_->Load("Resources/Model/Title/D", "D.obj");
	uint32_t aModelHandle = modelManager_->Load("Resources/Model/Title/A", "A.obj");
	uint32_t pModelHandle = modelManager_->Load("Resources/Model/Title/D", "D.obj");
	uint32_t exclamationModelHandle = modelManager_->Load("Resources/Model/Title/Exclamation","Exclamation.obj");
	gameNameObject_ = std::make_unique<GameNameObject>();
	gameNameObject_->Initialize(dModelHandle, aModelHandle, pModelHandle, exclamationModelHandle);

}

void TitleScene::Update(){

	//各シーンの更新
	baseTitleScene_->Update();
	//ゲーム名のオブジェクトの更新
	gameNameObject_->Update();
	//グラウンドのワールドトランスフォームの更新
	groundWorldTransform_.Update();
	groundMaterial_.Update();
	//カメラの更新
	camera_.Update();
	directionalLight_.Update();

	//処理が終わったら次のシーンへ
	if (isProcessEnd_) {
		gameSceneManager_->ChangeScene("Select");
	}

}

void TitleScene::DrawObject3D(){
	//各シーン
	baseTitleScene_->DrawObject3D(camera_, directionalLight_);
	//ゲーム名のオブジェクト
	gameNameObject_->DrawObject3D(camera_, directionalLight_);
	//地面
	groundModel_->Draw(groundWorldTransform_, camera_, groundMaterial_, directionalLight_);
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


