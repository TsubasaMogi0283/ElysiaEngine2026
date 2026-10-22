#include "TitleScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "CollisionCalculation.h"
#include "PushBackCalculation.h"
#include <AnimationManager.h>
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <TitleScene/Start/StartTitleScene.h>

TitleScene::TitleScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_->GetInstance();
}

void TitleScene::Initialize() {

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
	uint32_t pModelHandle = modelManager_->Load("Resources/Model/Title/P", "P.obj");
	uint32_t exclamationModelHandle = modelManager_->Load("Resources/Model/Title/Exclamation", "Exclamation.obj");
	gameNameObject_ = std::make_unique<GameNameObject>();
	gameNameObject_->Initialize(dModelHandle, aModelHandle, pModelHandle, exclamationModelHandle);

	//プレイヤー
	//モデル
	uint32_t playerModelhandle = modelManager_->Load("Resources/External/Model/human", "walk.gltf");
	//アニメーション
	uint32_t playerWalkAnimationHandle = animationManager_->Load("Resources/External/Model/human", "walk.gltf");
	std::vector<uint32_t> playerAnimationHandleVector;
	playerAnimationHandleVector.push_back(playerWalkAnimationHandle);
	//生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModelhandle, playerAnimationHandleVector);
	const float_t PLAYER_SCALE = 2.0f;
	player_->SetScale({ .x = PLAYER_SCALE,.y = PLAYER_SCALE,.z = PLAYER_SCALE });
	player_->SetRotate({ .x = 0.0f,.y = std::numbers::pi_v<float_t>,.z=0.0f });
	

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
	arrowWorldTransform_.translate = { .x = 9.0f,.y = 2.0f,.z = 0.0f };
	arrowWorldTransform_.rotate.y = std::numbers::pi_v<float_t>;
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
		arrowWorldTransform_.translate.x = 9.0f;
		arrowWorldTransform_.rotate.y = std::numbers::pi_v<float_t>;
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
		arrowWorldTransform_.rotate.y = 0.0f;
	}
	//各更新
	//プレイヤー
	player_->Update();
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
		//選曲
		if (isSelectStart_ && !isSelectEnd_) {
			gameSceneManager_->ChangeScene("Select");
			return;
		}
		//ゲーム終了
		else if (!isSelectStart_ && isSelectEnd_) {
			gameSceneManager_->SetIsGameEnd();
			return;
		}
		
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
	//プレイヤー
	player_->DrawObject3D(camera_, directionalLight_);

	//選択肢
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

