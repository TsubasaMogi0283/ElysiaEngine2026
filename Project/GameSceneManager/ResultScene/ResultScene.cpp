#include "ResultScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "LevelDataManager.h"
#include <ModelManager.h>
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <WindowsSetup.h>

ResultScene::ResultScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void ResultScene::Initialize(){
	//カメラ
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float_t> / 6.0f;
	camera_.translate = { .x = 0.0f,.y = 21.0f,.z = -40.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();


	//メインシーンで記録したものを取得
	musicInformation_ = gameSceneManager_->GetMusicInformation();
	temporaryRecievedRecord_ = gameSceneManager_->GetNoteJudgementResult();


#ifdef _DEBUG
	temporaryRecievedRecord_.perfect = 1000u;
	temporaryRecievedRecord_.great = 1000u;
	temporaryRecievedRecord_.good = 1000u;
	temporaryRecievedRecord_.miss = 1000u;

	temporaryRecievedRecord_.maxCombo = 1000u;
	temporaryRecievedRecord_.score = 1000000u;
	temporaryRecievedRecord_.gaugeRatio = 0.8f;
#endif // _DEBUG

	//下地
	baseSprite_ = Elysia::Sprite::Create();
	baseSprite_->SetScale({ .x = 0.75f,.y = 1.0f });
	baseSprite_->SetTransparency(0.25f);

	//タイトルのスプライト
	musicTitleSprite_ = Elysia::Sprite::Create();
	//作曲者のスプライト
	musicComposerSprite_ = Elysia::Sprite::Create();

	//ゲージ
	//テクスチャの読み込み
	uint32_t gaugeTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Gauge.png");
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Frame.png");
	gauge_ = std::make_unique<Gauge>();
	gauge_->Initialize(gaugeTextureHandle, frameTextureHandle);
	gauge_->SetTotalNotes(temporaryRecievedRecord_.totalNotes);
	gaugeMainScale_ = { .x = temporaryRecievedRecord_.gaugeRatio,.y = GAUGE_MAX_SCALE_.y };
	gauge_->SetAllScale(GAUGE_MAX_SCALE_);
	gauge_->SetMainScale(gaugeMainScale_);
	gauge_->SetInitialPosition({ .x = 100,.y=200 });
	//数字のテクスチャを読み込む
	const std::string NUMBER_PATH = "Resources/Sprite/Number/";
	for (uint8_t i = 0u;i < NUMBER_AMOUNT_;i++) {
		numberTextureHandle_[i] = textureManager_->Load(NUMBER_PATH + std::to_string(i) + ".png");
	}
	
	//数字のテクスチャの横幅を取得
	Vector2<int32_t> numberTextureSize = { 
		.x = static_cast<int32_t>(textureManager_->GetTextureWidth(numberTextureHandle_[0])),
		.y = static_cast<int32_t>(textureManager_->GetTextureHeight(numberTextureHandle_[0]))
	};
	

	//判定
	//0で初期化
	const uint8_t INITIAL_NUMBER_ = 0u;
	for (uint8_t i = 0u;i < ONE_HUNDRED_DIGIT_;i++) {
		perfect_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
		great_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
		good_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
		miss_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
	}
	
	//スコア
	for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
		score_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
	}
	//最大コンボ数
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxCombo_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER_]);
		//座標の設定
		maxCombo_[i].position = {
			.x = (ONE_THOUSAND_DIGIT_ - i - 3) * numberTextureSize.x + 640,
			.y = 0
		};
		maxCombo_[i].sprite->SetPosition(maxCombo_[i].position);
	}
}

void ResultScene::Update(){

	//各状態の処理を実行
	(this->*functionTable[static_cast<size_t>(currentState_)])();

	//更新
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

void ResultScene::DrawObject3D(){


}

void ResultScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void ResultScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void ResultScene::DrawSprite(){
	//下地用
	baseSprite_->Draw();

	//musicTitleSprite_->Draw();
	//musicComposerSprite_->Draw();
	//ゲージ
	gauge_->DrawSprite();

	//for (uint8_t i = 0u;i < ONE_HUNDRED_DIGIT_;i++) {
	//	perfect_[i].sprite->Draw(perfect_[i].value);
	//	great_[i].sprite->Draw(great_[i].value);
	//	good_[i].sprite->Draw(good_[i].value);
	//	miss_[i].sprite->Draw(miss_[i].value);
	//}
	//
	////スコア
	//for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
	//	score_[i].sprite->Draw(score_[i].value);
	//}
	////最大コンボ数
	//for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
	//	maxCombo_[i].sprite->Draw(maxCombo_[i].value);
	//}


}

void ResultScene::Open(){
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetOpenTransition()) {
		currentState_ = ResultSceneState::IncreaseValue;
	}
}

void ResultScene::IncreaseValue(){


	currentState_ = ResultSceneState::Display;

}

void ResultScene::Display(){
	if (input_->IsTriggerKey(DIK_SPACE)) {
		currentState_ = ResultSceneState::Return;
	}
}

void ResultScene::Return(){

}

void ResultScene::Close(){
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
		isEnd_ = true;
	}
	
}
