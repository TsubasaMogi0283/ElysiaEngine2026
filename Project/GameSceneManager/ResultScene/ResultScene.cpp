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

void ResultScene::Initialize() {
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

	//スコアボードの中心座標
	const float_t SCORE_BORD_SIZE = static_cast<float_t>(windowsSetup_->GetClientSize().x) * 0.75f;
	const int32_t SCORE_BOARD_CENTER_POSITION_X = static_cast<int32_t>(SCORE_BORD_SIZE) / 2;

	//ゲージ
	//テクスチャの読み込み
	uint32_t gaugeTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Gauge.png");
	uint32_t frameTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Frame.png");
	gauge_ = std::make_unique<Gauge>();
	gauge_->Initialize(gaugeTextureHandle, frameTextureHandle);
	gauge_->SetTotalNotes(temporaryRecievedRecord_.totalNotes);
	gaugeMainScale_ = { .x = GAUGE_MAX_SCALE_.x - (1.0f - temporaryRecievedRecord_.gaugeRatio),.y = GAUGE_MAX_SCALE_.y };
	gauge_->SetAllScale(GAUGE_MAX_SCALE_);
	gauge_->SetMainScale(gaugeMainScale_);
	gauge_->SetInitialPosition({ .x = SCORE_BOARD_CENTER_POSITION_X / 2-140 ,.y = 175 });
	//数字のテクスチャを読み込む
	const std::string NUMBER_PATH = "Resources/Sprite/Number/";
	for (uint8_t i = 0u; i < NUMBER_AMOUNT_; i++) {
		numberTextureHandle_[i] = textureManager_->Load(NUMBER_PATH + std::to_string(i) + ".png");
	}

	//数字のテクスチャの横幅を取得
	Vector2<int32_t> numberTextureSize = {
		.x = static_cast<int32_t>(textureManager_->GetTextureWidth(numberTextureHandle_[0])),
		.y = static_cast<int32_t>(textureManager_->GetTextureHeight(numberTextureHandle_[0]))
	};

	

	//判定のテクスチャの読み込み
	std::array<uint32_t, static_cast<size_t>(NoteJudgement::Selection::Size)> judgementTextureHandle = {};
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Miss)] = textureManager_->Load("Resources/Sprite/Judgement/Miss.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Good)] = textureManager_->Load("Resources/Sprite/Judgement/Good.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Great)] = textureManager_->Load("Resources/Sprite/Judgement/Great.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Perfect)] = textureManager_->Load("Resources/Sprite/Judgement/Perfect.png");
	//サイズを取得
	Vector2<int32_t> judgementTextureSize = {
		.x = static_cast<int32_t>(textureManager_->GetTextureWidth(judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Miss)])),
		.y = static_cast<int32_t>(textureManager_->GetTextureHeight(judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Miss)])),
	};

	//判定のY座標
	const int32_t JUDGEMENT_POSITION_Y = 210;
	//文字の座標
	const int32_t LEFT_JUDGEMENT_POSITIN_X = 50;
	//判定のオフセットX座標
	const int32_t JUDGEMENT_OFFSET_POSITION_Y = 10;
	//文字と値の間隔
	const int32_t DIDIT_INTERVALPOSITION_X = 100;
	//コンボの座標(記録用)
	Vector2<int32_t> comboPosition = {};
	//判定の文字のスプライト
	for (size_t i = 0u; i < static_cast<size_t>(NoteJudgement::Selection::Size); i++) {
		//スプライトの生成
		judgementSpriteArray_[i] = Elysia::Sprite::Create(judgementTextureHandle[i]);
		//スケールの設定
		judgementSpriteArray_[i]->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });

		//座標の設定
		Vector2<int32_t> position = {
			.x = LEFT_JUDGEMENT_POSITIN_X,
			.y = JUDGEMENT_POSITION_Y + (judgementTextureSize.y + JUDGEMENT_OFFSET_POSITION_Y) * static_cast<int32_t>(static_cast<size_t>(NoteJudgement::Selection::Size) - i)
		};
		judgementSpriteArray_[i]->SetPosition(position);

		//コンボの座標
		if (i == static_cast<size_t>(NoteJudgement::Selection::Miss)) {
			comboPosition = {
				.x = static_cast<int32_t>(static_cast<float_t>(numberTextureSize.x) * static_cast<float_t>(ONE_MILLION_DIGIT_ - i) * scoreScale_) +100,
				.y = position.y + judgementTextureSize.y
			};
		}
	}

	//判定
	//0で初期化
	const uint8_t INITIAL_NUMBER = 0u;
	for (uint8_t i = 0u; i < ONE_THOUSAND_DIGIT_; i++) {
		perfectDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		greatDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		goodDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		missDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);

		//座標の設定
		Vector2<int32_t> position = {
			.x = (ONE_THOUSAND_DIGIT_ - i - 3) * numberTextureSize.x + LEFT_JUDGEMENT_POSITIN_X + judgementTextureSize.x + DIDIT_INTERVALPOSITION_X,
			.y = JUDGEMENT_POSITION_Y + (judgementTextureSize.y + JUDGEMENT_OFFSET_POSITION_Y) * static_cast<int32_t>(static_cast<size_t>(NoteJudgement::Selection::Size) - i)
		};
		perfectDigit_[i].sprite->SetPosition(position);
		greatDigit_[i].sprite->SetPosition(position);
		goodDigit_[i].sprite->SetPosition(position);
		missDigit_[i].sprite->SetPosition(position);

		//0で初期化
		perfectDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
		greatDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
		goodDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
		missDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];


	}



	//最大コンボ数
	uint32_t comboTextureHandle = textureManager_->Load("Resources/Sprite/Judgement/Combo.png");
	maxComboSprite_ = Elysia::Sprite::Create(comboTextureHandle);
	maxComboSprite_->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });
	maxComboSprite_->SetPosition({ .x = LEFT_JUDGEMENT_POSITIN_X ,.y = comboPosition.y });
	//各桁
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxComboDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		//座標の設定
		maxComboDigit_[i].position = {
			.x = (ONE_THOUSAND_DIGIT_ - i - 3) * numberTextureSize.x + LEFT_JUDGEMENT_POSITIN_X+ judgementTextureSize.x+ DIDIT_INTERVALPOSITION_X,
			.y = comboPosition.y
		};
		maxComboDigit_[i].sprite->SetPosition(maxComboDigit_[i].position);
	}
	
	//スコア
	for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
		score_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		score_[i].sprite->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
		//座標の設定
		Vector2<int32_t>digitPosition = { 
			.x = static_cast<int32_t>(static_cast<float_t>(numberTextureSize.x) * static_cast<float_t>(ONE_MILLION_DIGIT_ - i) * scoreScale_) + SCORE_BOARD_CENTER_POSITION_X/2,
			.y = 250 
		};
		score_[i].sprite->SetPosition(digitPosition);
		score_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
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

	
	
	//スコア
	for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
		score_[i].sprite->Draw(score_[i].textureHandle);
	}

	//最大コンボ数
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxComboDigit_[i].sprite->Draw(maxComboDigit_[i].value);
	}
	maxComboSprite_->Draw();
	
	//判定
	for (size_t i = 0u; i < static_cast<size_t>(NoteJudgement::Selection::Size); i++) {
		judgementSpriteArray_[i]->Draw();
	}
	//桁
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		perfectDigit_[i].sprite->Draw(perfectDigit_[i].textureHandle);
		greatDigit_[i].sprite->Draw(greatDigit_[i].textureHandle);
		goodDigit_[i].sprite->Draw(goodDigit_[i].textureHandle);
		missDigit_[i].sprite->Draw(missDigit_[i].textureHandle);
	}

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
