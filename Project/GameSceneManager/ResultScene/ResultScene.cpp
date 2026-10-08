#include "ResultScene.h"

#include <imgui.h>
#include <numbers>

#include <Easing.h>
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
	temporaryRecievedRecord_.score = 1234567u;
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
	gauge_->SetAllScale(GAUGE_MAX_SCALE_);
	gauge_->SetMainScale({ .x = 0.0f,.y = 0.0f });
	gauge_->SetInitialPosition({ .x = SCORE_BOARD_CENTER_POSITION_X / 2-140 ,.y = 225 });
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

	//判定の座標
	const Vector2<int32_t> JUDGEMENT_POSITION = { .x = 50,.y = 300 };
	//判定のオフセットX座標
	const int32_t JUDGEMENT_OFFSET_POSITION_Y = 10;
	//文字と値の間隔
	const int32_t DIDIT_INTERVAL_POSITION_X = 100;
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
			.x = JUDGEMENT_POSITION.x,
			.y = JUDGEMENT_POSITION.y + (judgementTextureSize.y + JUDGEMENT_OFFSET_POSITION_Y) * static_cast<int32_t>(i)
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
	for (uint8_t j = 0u; j < static_cast<uint8_t>(NoteJudgement::Selection::Size);j++) {
		Vector2<int32_t>position = {};
		position.y = JUDGEMENT_POSITION.y + (judgementTextureSize.y + 8) * static_cast<int32_t>(j);
		for (uint8_t i = 0u; i < ONE_THOUSAND_DIGIT_; i++) {
			//生成
			judgementDigitArray_[j][i].sprite=Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
			judgementDigitArray_[j][i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
			position.x = (ONE_THOUSAND_DIGIT_ - i - 3) * numberTextureSize.x + JUDGEMENT_POSITION.x + judgementTextureSize.x + DIDIT_INTERVAL_POSITION_X;
			//0で初期化
			judgementDigitArray_[j][i].sprite->SetPosition(position);
		}
		judgementIncreaseTimeArray_[j] = MAX_JUDGEMENT_INCREASE_TIME_ * static_cast<float_t>(j + 1u);
	}

	
	//最大コンボ数
	uint32_t comboTextureHandle = textureManager_->Load("Resources/Sprite/Judgement/Combo.png");
	maxComboSprite_ = Elysia::Sprite::Create(comboTextureHandle);
	maxComboSprite_->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });
	maxComboSprite_->SetPosition({ .x = JUDGEMENT_POSITION.x ,.y = comboPosition.y });
	//各桁
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxComboDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		//座標の設定
		maxComboDigit_[i].position = {
			.x = (ONE_THOUSAND_DIGIT_ - i - 3) * numberTextureSize.x + JUDGEMENT_POSITION .x+ judgementTextureSize.x+ DIDIT_INTERVAL_POSITION_X,
			.y = comboPosition.y
		};
		maxComboDigit_[i].sprite->SetPosition(maxComboDigit_[i].position);
	}

	//スコア
	for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
		scoreDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		scoreDigit_[i].sprite->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
		//座標の設定
		Vector2<int32_t>digitPosition = { 
			.x = static_cast<int32_t>(static_cast<float_t>(numberTextureSize.x) * static_cast<float_t>(ONE_MILLION_DIGIT_ - i) * scoreScale_) + SCORE_BOARD_CENTER_POSITION_X/2,
			.y = 175 
		};
		scoreDigit_[i].sprite->SetPosition(digitPosition);
		scoreDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
	}
	
	//ランク
	uint32_t rankTexturehHandle[static_cast<uint8_t>(RankSelection::Size)] = {};
	rankTexturehHandle[static_cast<uint8_t>(RankSelection::S)] = textureManager_->Load("Resources/Sprite/Result/Rank/S.png");
	rankTexturehHandle[static_cast<uint8_t>(RankSelection::A)] = textureManager_->Load("Resources/Sprite/Result/Rank/A.png");
	rankTexturehHandle[static_cast<uint8_t>(RankSelection::B)] = textureManager_->Load("Resources/Sprite/Result/Rank/B.png");
	rankTexturehHandle[static_cast<uint8_t>(RankSelection::C)] = textureManager_->Load("Resources/Sprite/Result/Rank/C.png");
	rankTexturehHandle[static_cast<uint8_t>(RankSelection::D)] = textureManager_->Load("Resources/Sprite/Result/Rank/D.png");
	//生成
	rankSprite_ = Elysia::Sprite::Create(rankTexturehHandle[static_cast<uint8_t>(RankSelection::S)]);
	rankSprite_->SetInvisible(true);
	rankSprite_->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
	rankSprite_->SetPosition({ .x = SCORE_BOARD_CENTER_POSITION_X + 250,.y = 475 });
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
		scoreDigit_[i].sprite->Draw(scoreDigit_[i].textureHandle);
	}

	//最大コンボ数
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxComboDigit_[i].sprite->Draw(maxComboDigit_[i].textureHandle);
	}
	maxComboSprite_->Draw();
	
	//判定
	for (size_t i = 0u; i < static_cast<size_t>(NoteJudgement::Selection::Size); i++) {
		judgementSpriteArray_[i]->Draw();
	}
	//桁
	for (uint8_t j = 0u; j < static_cast<uint8_t>(NoteJudgement::Selection::Size);j++) {
		for (uint8_t i = 0u; i < ONE_THOUSAND_DIGIT_; i++) {
			judgementDigitArray_[j][i].sprite->Draw(judgementDigitArray_[j][i].textureHandle);
		}
	}

	//ランクの表示
	rankSprite_->Draw();
}

void ResultScene::Open(){
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetOpenTransition()) {
		currentState_ = ResultSceneState::IncreaseGauge;
	}
}

void ResultScene::IncreaseGauge(){
	//ゲージの上昇
	gaugeIncreaseTime_ += DELTA_TIME_;
	float_t t = SingleCalculation::InverseLerp(0.0f, MAX_GAUGE_INCREASE_TIME_, gaugeIncreaseTime_);
	t=std::clamp(t, 0.0f, 1.0f);
	gauge_->SetMainScale({ .x = temporaryRecievedRecord_.gaugeRatio * Easing::EaseOutQuart(t)* GAUGE_MAX_SCALE_.x,.y = 1.0f });

	//値の増加へ
	if (t >= 1.0f) {
		currentState_ = ResultSceneState::IncreaseValue;
	}
}

void ResultScene::IncreaseValue(){

	//スコアの上昇
	if (!isEndIncreaseScore_) {
		scoreIncreaseTime_ += DELTA_TIME_;
		float_t t = SingleCalculation::InverseLerp(0.0f, MAX_GAUGE_INCREASE_TIME_, scoreIncreaseTime_);
		t = std::clamp(t, 0.0f, 1.0f);
		//各桁の値を求めていく
		uint32_t score = static_cast<uint32_t>(static_cast<float_t>(temporaryRecievedRecord_.score * t));
		for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
			//各桁の数字を求める
			uint8_t digit = score % 10;
			scoreDigit_[i].textureHandle = numberTextureHandle_[static_cast<uint16_t>(digit)];
			//10で割っていく
			score /= 10;
		}
		if (t >= 1.0f) {
			isEndIncreaseScore_ = true;
		}
	}
	//判定の上昇
	else {
		judgementIncreaseTime += DELTA_TIME_;
	}


	temporaryRecievedRecord_.perfect = 1000u;
	temporaryRecievedRecord_.great = 1000u;
	temporaryRecievedRecord_.good = 1000u;
	temporaryRecievedRecord_.miss = 1000u;
	temporaryRecievedRecord_.maxCombo = 1000u;

	//currentState_ = ResultSceneState::Display;

	
}

void ResultScene::MoveRank(){
	//ランク表示
	rankSprite_->SetInvisible(false);
}

void ResultScene::Display(){
	if (input_->IsTriggerKey(DIK_SPACE)) {
		currentState_ = ResultSceneState::Return;
	}
}

void ResultScene::Return(){
	currentState_ = ResultSceneState::Close;
}

void ResultScene::Close(){
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
		isEnd_ = true;
	}
	
}
