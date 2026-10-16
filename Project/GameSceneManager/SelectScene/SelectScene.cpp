#include "SelectScene.h"

#include <imgui.h>
#include <numbers>
#include <algorithm>

#include "Input.h"
#include "ModelManager.h"
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <StringOption.h>

SelectScene::SelectScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
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
	allScoreData_ = gameSceneManager_->GetScoreDataManager()->GetAllMusciScoreData();

	std::vector<ExistMusicScoreData> existData = gameSceneManager_->GetScoreDataManager()->GetExistScoreData();
	for (size_t i = 0u;i < existData.size();i++) {
		std::string path = "Resources/MusicScore/" + existData[i].musicName;
		std::string extention= StringOption::FindExtension(path, existData[i].musicName + "_Jacket");
		uint32_t jacketTextureHandle = textureManager_->Load(path + "/"+existData[i].musicName + "_Jacket" + extention);
		jacketTextureHandleVector.push_back(jacketTextureHandle);
	}
	//フレーム
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Jacket/DifficultyFrame.png");

	//初期化
	jacket_->Initialize(jacketTextureHandleVector, frameTextureHandle);
	jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));

}

void SelectScene::Update(){

	if (gameSceneManager_->GetTransition()->SetOpenTransition()) {
		isOpendTransiont_ = true;
	}
	if (isOpendTransiont_) {

		//楽曲
		//右に選択
		if (input_->IsTriggerKey(DIK_D) || input_->IsTriggerKey(DIK_RIGHT)) {
			jacket_->SetIsMoveToRight(true);
		}
		//左に選択
		else if (input_->IsTriggerKey(DIK_A) || input_->IsTriggerKey(DIK_LEFT)) {
			jacket_->SetIsMoveToLeft(true);
		}

		//難易度の設定
		//上を選択
		if (input_->IsTriggerKey(DIK_W) || input_->IsTriggerKey(DIK_UP)) {
			currentDifficultyNumber_++;
		}
		//下を選択
		if (input_->IsTriggerKey(DIK_S) || input_->IsTriggerKey(DIK_DOWN)) {
			currentDifficultyNumber_--;
		}
		//簡単から難しいまでにする
		currentDifficultyNumber_ = std::clamp(currentDifficultyNumber_, static_cast<int8_t>(Difficulty::Easy), static_cast<int8_t>(Difficulty::Hard));
		jacket_->SetDifficulty(static_cast<Difficulty>(currentDifficultyNumber_));

		//決定
		if (input_->IsTriggerKey(DIK_SPACE)) {
			jacket_->SetIsDecide();
			//選択した楽曲のデータを取得
			gameSceneManager_->SetMusicScoreData(allScoreData_[currentSelectedMusicNumber_]);
			
			
			

		}

		//処理終了
		if (jacket_->GetIsProcessEnd()) {
			//待機時間
			waitingTime_ += DELTA_TIME_;
		}
	}

	//ジャケット
	jacket_->Update();
	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();
	//現在選択している楽曲番号を取得
	currentSelectedMusicNumber_ = jacket_->GetCurrentSelectMusicNumber();

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


