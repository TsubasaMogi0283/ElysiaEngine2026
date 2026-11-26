#include "SelectScene.h"

#include <imgui.h>
#include <numbers>
#include <algorithm>

#include "Input.h"
#include "ModelManager.h"
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <MiniAudio.h>
#include <WindowsSetup.h>
#include <StringOption.h>
#include "Music/MusicSelectScene.h"

SelectScene::SelectScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//オーディオ
	miniAudio_ = Elysia::MiniAudio::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
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
	
	//テクスチャの読みこみ
	std::vector<uint32_t>jacketTextureHandleVector = {};
	
	//全ての楽曲データを取得
	allScoreData_ = gameSceneManager_->GetScoreDataManager()->GetExistScoreData();
	for (size_t i = 0u;i < allScoreData_.size();i++) {
		jacketTextureHandleVector.push_back(allScoreData_[i].jacketTextureHandle);
	}
	//フレーム
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Jacket/DifficultyFrame.png");
	Vector2<float_t>frameTextureSize = textureManager_->GetTextureSize(frameTextureHandle);
	//初期化
	jacket_->Initialize(jacketTextureHandleVector, frameTextureHandle);
	jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));

	//決定
	uint32_t decideTexturehandle = textureManager_->Load("Resources/Sprite/Select/Decide.png");
	decideSprite_ = Elysia::Sprite::Create(decideTexturehandle);
	decideSprite_->SetScale({ .x = 0.8f,.y = 0.8f });
	decideSprite_->SetPosition({ .x = 1050,.y = 500 });
	//戻る
	uint32_t backTexturehandle = textureManager_->Load("Resources/Sprite/Select/Back.png");
	backSprite_ = Elysia::Sprite::Create(backTexturehandle);
	backSprite_->SetScale({ .x = 0.8f,.y = 0.8f });
	backSprite_->SetPosition({ .x = 10,.y = 500 });

	//効果音の読み込み
	decideSEHandle_ = miniAudio_->Load("Resources/Audio/SE/Deside.wav");
	selectSEHandle_ = miniAudio_->Load("Resources/Audio/SE/Select.wav");

	//楽曲選択シーンから始める
	baseSelectScene_ = std::make_unique<MusicSelectScene>();
	//初期化
	baseSelectScene_->SetSelectScene(this);
	baseSelectScene_->Initialize();

}

void SelectScene::Update(){

	//ジャケット
	jacket_->Update();
	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();
	//各シーンの更新
	baseSelectScene_->Update();

	//メインシーン
	if (isEnd_) {
		if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
			//楽曲を止める
			miniAudio_->Stop(allScoreData_[playingMusicNumber_].musicHandle);
			gameSceneManager_->ChangeScene("Main");
			return;
		}
		
	}

#ifdef _DEBUG
	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG

}

void SelectScene::DrawObject3D(){
	//各シーンのオブジェクトの描画
	baseSelectScene_->DrawObject3D(camera_, directionalLight_);
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
	//各シーンのスプライトの描画
	baseSelectScene_->DrawSprite();
	//決定
	decideSprite_->Draw();
	//戻る
	backSprite_->Draw();
}