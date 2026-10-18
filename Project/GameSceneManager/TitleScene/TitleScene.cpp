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


	//開始
	uint32_t startModelHandle = modelManager_->Load("Resources/Model/Title/Start","Start.obj");
	startModel_=Elysia::Model::Create(startModelHandle);
	startWorldTransform_.Initialize();
	startWorldTransform_.translate = { .x = 5.0f,.y = SELECT_POSITION_Y_,.z = 0.0f };
	startMaterial_.Initialize();
	startMaterial_.color.w = 0.0f;

	//終了
	uint32_t endModelHandle = modelManager_->Load("Resources/Model/Title/End", "End.obj");
	endModel_ = Elysia::Model::Create(endModelHandle);
	endWorldTransform_.Initialize();
	endWorldTransform_.translate= { .x = -4.0f,.y = SELECT_POSITION_Y_,.z = 0.0f };
	endMaterial_.Initialize();
	endMaterial_.color.w = 0.0f;

	//矢印
	uint32_t arrowModelHandle = modelManager_->Load("Resources/Model/Title/SelectArrow", "SelectArrow.obj");
	arrowModel_ = Elysia::Model::Create(arrowModelHandle);
	arrowWorldTransform_.Initialize();
	arrowWorldTransform_.translate = { .x = 0.0f,.y = 2.0f,.z = 0.0f };
	arrowMaterial_.Initialize();
	arrowMaterial_.color.w = 0.0f;
}

void TitleScene::Update(){
	//開始を選択時
	if (isSelectStart_&& !isSelectEnd_) {
		startWorldTransform_.scale = { .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ ,.z = SELECTED_SCALE_ };
		endWorldTransform_.scale = { .x = NO_SELECTED_SCALE_ ,.y = NO_SELECTED_SCALE_ ,.z = NO_SELECTED_SCALE_ };
		//回転
		startWorldTransform_.rotate.y -= ARROW_ROTATE_AMOUNT_;
		endWorldTransform_.rotate.y = 0.0f;
		//矢印の座標
		arrowWorldTransform_.translate.x = 0.0f;
	}
	//終了を選択時
	if (!isSelectStart_ && isSelectEnd_) {
		startWorldTransform_.scale = { .x = NO_SELECTED_SCALE_ ,.y = NO_SELECTED_SCALE_ ,.z = NO_SELECTED_SCALE_ };
		endWorldTransform_.scale = { .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ ,.z = SELECTED_SCALE_ };
		//回転
		startWorldTransform_.rotate.y = 0.0f;
		endWorldTransform_.rotate.y -= ARROW_ROTATE_AMOUNT_;
		//矢印の座標
		arrowWorldTransform_.translate.x = -9.0f;
	}

	//各シーンの更新
	baseTitleScene_->Update();
	//ゲーム名のオブジェクトの更新
	gameNameObject_->Update();
	//グラウンドの更新
	groundWorldTransform_.Update();
	groundMaterial_.Update();
	//開始の更新
	startWorldTransform_.Update();
	startMaterial_.Update();
	//終了
	endWorldTransform_.Update();
	endMaterial_.Update();
	//矢印
	arrowWorldTransform_.rotate.x = arrowRotateX_;
	arrowWorldTransform_.Update();
	arrowMaterial_.Update();

	//カメラの更新
	camera_.Update();
	directionalLight_.Update();

	//処理が終わったら次のシーンへ
	if (isProcessEnd_) {
		gameSceneManager_->ChangeScene("Select");
		return;
	}


#ifdef _DEBUG
	ImGui::Begin("タイトル");
	ImGui::SliderFloat3("開始座標", &startWorldTransform_.translate.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("終了座標", &endWorldTransform_.translate.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("矢印座標", &arrowWorldTransform_.translate.x, -30.0f, 30.0f);
	ImGui::InputFloat("hh", &arrowRotateX_);
	ImGui::End();
#endif // _DEBUG

}

void TitleScene::DrawObject3D(){
	//各シーン
	baseTitleScene_->DrawObject3D(camera_, directionalLight_);
	//ゲーム名のオブジェクト
	gameNameObject_->DrawObject3D(camera_, directionalLight_);
	//地面
	groundModel_->Draw(groundWorldTransform_, camera_, groundMaterial_, directionalLight_);

	startModel_->Draw(startWorldTransform_, camera_, startMaterial_, directionalLight_);
	endModel_->Draw(endWorldTransform_, camera_, endMaterial_, directionalLight_);
	arrowModel_->Draw(arrowWorldTransform_, camera_, arrowMaterial_, directionalLight_);

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

