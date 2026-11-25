#include "ResultScene.h"

#include <imgui.h>
#include <numbers>
#include <magic_enum/magic_enum.hpp>

#include <Easing.h>
#include "Input.h"
#include "LevelDataManager.h"
#include <ModelManager.h>
#include <GameSceneManager.h>
#include <TextureManager.h>
#include <WindowsSetup.h>

ResultScene::ResultScene() {
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
	musicScoreData_ = gameSceneManager_->GetTemporaryMusicScoreData().musicScoreData[gameSceneManager_->GetTemporaryDifficulty()];
	temporaryRecievedRecord_ = gameSceneManager_->GetNoteJudgementResult();


	//下地
	baseSprite_ = Elysia::Sprite::Create();
	baseSprite_->SetScale({ .x = 0.9f,.y = 0.65f });
	baseSprite_->SetAnchorPoint({ .x = 0.5f,.y = 0.0f });
	baseSprite_->SetPosition({ .x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 2u),.y = 20.0f });
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
	gauge_->SetAllScale(GAUGE_MAX_SCALE_);
	gauge_->SetMainScale({ .x = 0.0f,.y = 0.0f });
	gauge_->SetPosition({ .x = static_cast<float_t>(windowsSetup_->GetClientSize().x / 4u) - 50.0f,.y = 125.0f });
	//数字のテクスチャを読み込む
	const std::string NUMBER_PATH = "Resources/Sprite/Number/";
	for (uint8_t i = 0u; i < NUMBER_AMOUNT_; i++) {
		numberTextureHandle_[i] = textureManager_->Load(NUMBER_PATH + std::to_string(i) + ".png");
	}

	//数字のテクスチャの横幅を取得
	Vector2<float_t> numberTextureSize = textureManager_->GetTextureSize(numberTextureHandle_[0]);

	//判定のテクスチャの読み込み
	std::array<uint32_t, static_cast<size_t>(NoteJudgement::Selection::Size)> judgementTextureHandle = {};
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Miss)] = textureManager_->Load("Resources/Sprite/Judgement/Miss.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Good)] = textureManager_->Load("Resources/Sprite/Judgement/Good.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Great)] = textureManager_->Load("Resources/Sprite/Judgement/Great.png");
	judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Perfect)] = textureManager_->Load("Resources/Sprite/Judgement/Perfect.png");
	//サイズを取得
	Vector2<float_t> judgementTextureSize = textureManager_->GetTextureSize(judgementTextureHandle[static_cast<size_t>(NoteJudgement::Selection::Miss)]);

	//判定の座標
	const Vector2<float_t> JUDGEMENT_POSITION = { .x = 100.0f,.y = 200.0f };
	//判定のオフセットX座標
	const float_t JUDGEMENT_OFFSET_POSITION_Y = 10.0f;
	//0で初期化
	const uint8_t INITIAL_NUMBER = 0u;
	//桁の座標オフセット
	const float_t DIGIT_OFFSET_POSITION_X_ = -100.0f;

	Vector2<float_t> judgementTexturePosition = {};
	//判定の文字のスプライト
	for (size_t i = 0u; i < static_cast<size_t>(NoteJudgement::Selection::Size); i++) {
		//スプライトの生成
		judgementInformation_[i].textSprite = Elysia::Sprite::Create(judgementTextureHandle[i]);
		//スケールの設定
		judgementInformation_[i].textSprite->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });

		//座標の設定
		judgementTexturePosition = {
			.x = JUDGEMENT_POSITION.x,
			.y = JUDGEMENT_POSITION.y + (judgementTextureSize.y + JUDGEMENT_OFFSET_POSITION_Y) * static_cast<float_t>(i) * JUDGEMENT_TEXTURE_SCALE_
		};
		judgementInformation_[i].textSprite->SetPosition(judgementTexturePosition);

		//各桁
		Vector2<float_t> digitPosition = {};
		digitPosition.y = JUDGEMENT_POSITION.y + (judgementTextureSize.y + JUDGEMENT_OFFSET_POSITION_Y) * static_cast<float_t>(i) * JUDGEMENT_TEXTURE_SCALE_;
		for (uint8_t j = 0u; j < ONE_THOUSAND_DIGIT_; j++) {
			//生成
			judgementInformation_[i].valueDigitInformationArray_[j].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
			judgementInformation_[i].valueDigitInformationArray_[j].textureHandle = numberTextureHandle_[INITIAL_NUMBER];

			//スケールの設定
			judgementInformation_[i].valueDigitInformationArray_[j].sprite->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });
			//座標の設定
			digitPosition.x = JUDGEMENT_POSITION.x + judgementTextureSize.x + static_cast<float_t>(ONE_THOUSAND_DIGIT_ - j) * numberTextureSize.x * JUDGEMENT_TEXTURE_SCALE_ + DIGIT_OFFSET_POSITION_X_;
			judgementInformation_[i].valueDigitInformationArray_[j].sprite->SetPosition(digitPosition);
		}
		//増加開始時間
		judgementInformation_[i].increaseStartTime = MAX_JUDGEMENT_INCREASE_TIME_ * static_cast<float_t>(i + 1u) - 0.5f;
		//増加終了時間
		judgementInformation_[i].increaseEndTime = judgementInformation_[i].increaseStartTime + MAX_JUDGEMENT_INCREASE_TIME_;
	}

	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Perfect)].value = temporaryRecievedRecord_.perfect;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Great)].value = temporaryRecievedRecord_.great;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Good)].value = temporaryRecievedRecord_.good;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Miss)].value = temporaryRecievedRecord_.miss;

	//コンボのテクスチャを読み込み
	uint32_t comboTextureHandle = textureManager_->Load("Resources/Sprite/Judgement/Combo.png");

	//最大コンボ
	//生成
	maxComboSprite_ = Elysia::Sprite::Create(comboTextureHandle);
	maxComboSprite_->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });
	maxComboSprite_->SetPosition({ .x = JUDGEMENT_POSITION.x ,.y = judgementTexturePosition.y + judgementTextureSize.y * JUDGEMENT_TEXTURE_SCALE_ });
	//各桁
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		//生成
		maxComboDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		//スケールの設定
		maxComboDigit_[i].sprite->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_,.y = JUDGEMENT_TEXTURE_SCALE_ });
		//座標の設定
		maxComboDigit_[i].position = {
			.x = JUDGEMENT_POSITION.x + judgementTextureSize.x + static_cast<float_t>(ONE_THOUSAND_DIGIT_ - i) * numberTextureSize.x * JUDGEMENT_TEXTURE_SCALE_ + DIGIT_OFFSET_POSITION_X_,
			.y = judgementTexturePosition.y + judgementTextureSize.y * JUDGEMENT_TEXTURE_SCALE_
		};
		maxComboDigit_[i].sprite->SetPosition(maxComboDigit_[i].position);
		//テクスチャハンドルの設定
		maxComboDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
	}

	//スコア
	for (uint8_t i = 0u;i < ONE_MILLION_DIGIT_;i++) {
		//設定
		scoreDigit_[i].sprite = Elysia::Sprite::Create(numberTextureHandle_[INITIAL_NUMBER]);
		//スケールの設定
		scoreDigit_[i].sprite->SetScale({ .x = JUDGEMENT_TEXTURE_SCALE_ ,.y = JUDGEMENT_TEXTURE_SCALE_ });
		//座標の設定
		Vector2<float_t>digitPosition = {
			.x = numberTextureSize.x * static_cast<float_t>(ONE_MILLION_DIGIT_ - i) * scoreScale_ * JUDGEMENT_TEXTURE_SCALE_ + 700.0f,
			.y = JUDGEMENT_POSITION.y
		};
		scoreDigit_[i].sprite->SetPosition(digitPosition);
		//テクスチャハンドル
		scoreDigit_[i].textureHandle = numberTextureHandle_[INITIAL_NUMBER];
	}


#ifdef _DEBUG
	temporaryRecievedRecord_.totalNotes = 1000u;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Perfect)].value = 500;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Great)].value = 300;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Good)].value = 150;
	judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Miss)].value = 50;
	temporaryRecievedRecord_.maxCombo = 1234u;
#endif // _DEBUG

	//ランク
	//テクスチャ読み込み
	uint32_t rankTexturehHandle[static_cast<uint8_t>(RankSelection::Size)] = {};
	//Sから始める
	for (uint8_t i = static_cast<uint8_t>(RankSelection::S);i < static_cast<uint8_t>(RankSelection::Size);i++) {
		std::string_view rank = magic_enum::enum_name(static_cast<RankSelection>(i));
		std::string newString{ rank };
		const std::string PATH = "Resources/Sprite/Result/Rank/" + newString + ".png";
		rankTexturehHandle[static_cast<uint8_t>(i)] = textureManager_->Load(PATH);
	}

	uint16_t allPerfectJudgementScore = temporaryRecievedRecord_.totalNotes*3u;
	const uint16_t GREAT_DECREASE_SCALE = judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Great)].value * static_cast<uint16_t>(NoteJudgement::Selection::Great);
	const uint16_t GOOD_DECREASE_SCALE = judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Good)].value * static_cast<uint16_t>(NoteJudgement::Selection::Good);
	const uint16_t MISS_DECREASE_SCALE = judgementInformation_[static_cast<uint16_t>(NoteJudgement::Selection::Miss)].value * static_cast<uint16_t>(NoteJudgement::Selection::Miss);
	uint16_t judgementScore = allPerfectJudgementScore - (GREAT_DECREASE_SCALE + GOOD_DECREASE_SCALE + MISS_DECREASE_SCALE);

	//ランク
	rankSprite_ = Elysia::Sprite::Create(rankTexturehHandle[static_cast<uint8_t>(RankSelection::S)]);
	rankSprite_->SetInvisible(true);
	rankSprite_->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
	rankSprite_->SetPosition({ .x = static_cast<float_t>(windowsSetup_->GetClientSize().x) / 2.0f + 200.0f,.y = 350.0f });

	//決定
	uint32_t decideTexturehandle = textureManager_->Load("Resources/Sprite/Select/Decide.png");
	decideSprite_ = Elysia::Sprite::Create(decideTexturehandle);
	decideSprite_->SetScale({ .x = 0.8f,.y = 0.8f });
	decideSprite_->SetPosition({ .x = 1050,.y = 500 });
	decideSprite_->SetInvisible(true);

#ifdef _DEBUG
	temporaryRecievedRecord_.gaugeRatio = static_cast<float_t>(judgementScore) / static_cast<float_t>(allPerfectJudgementScore);
	temporaryRecievedRecord_.score = 123456u;
	temporaryRecievedRecord_.combo = 1234u;
#endif // _DEBUG

}

void ResultScene::Update() {

	//各状態の処理を実行
	(this->*functionTable[static_cast<size_t>(currentState_)])();

	//更新
	//カメラ
	camera_.Update();
	//平行光源
	directionalLight_.Update();


	if (isEnd_) {
		//記録の初期化
		gameSceneManager_->ResetRecord();
		//セレクトへ
		gameSceneManager_->ChangeScene("Select");
		return;
	}

#ifdef _DEBUG
	ImGui::Begin("リザルトシーン");

	ImGui::End();


#endif // _DEBUG
}

void ResultScene::DrawObject3D() {


}

void ResultScene::PreDrawPostEffect() {
	//描画前処理
	backTexture_->PreDraw();
}

void ResultScene::DrawPostEffect() {
	//描画前処理
	backTexture_->Draw();
}

void ResultScene::DrawSprite() {
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
	maxComboSprite_->Draw();
	//桁
	for (uint8_t i = 0u;i < ONE_THOUSAND_DIGIT_;i++) {
		maxComboDigit_[i].sprite->Draw(maxComboDigit_[i].textureHandle);
	}

	//判定
	for (size_t i = 0u; i < static_cast<size_t>(NoteJudgement::Selection::Size); i++) {
		//テキスト
		judgementInformation_[i].textSprite->Draw();
		//各桁の数字
		for (uint8_t j = 0u; j < ONE_THOUSAND_DIGIT_; j++) {
			judgementInformation_[i].valueDigitInformationArray_[j].sprite->Draw(judgementInformation_[i].valueDigitInformationArray_[j].textureHandle);
		}
	}

	//ランクの表示
	rankSprite_->Draw();

	//決定ボタン
	decideSprite_->Draw();
}

void ResultScene::Open() {
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetOpenTransition()) {
		currentState_ = ResultSceneState::IncreaseGauge;
	}
}

void ResultScene::IncreaseGauge() {
	//ゲージの上昇
	gaugeIncreaseTime_ += DELTA_TIME_;
	float_t t = SingleCalculation::InverseLerp(0.0f, MAX_GAUGE_INCREASE_TIME_, gaugeIncreaseTime_);
	t = std::clamp(t, 0.0f, 1.0f);
	gauge_->SetMainScale({ .x = temporaryRecievedRecord_.gaugeRatio * Easing::EaseOutQuart(t) * GAUGE_MAX_SCALE_.x,.y = 1.0f });

	//値の増加へ
	if (t >= 1.0f) {
		currentState_ = ResultSceneState::IncreaseJudgement;
	}
}

void ResultScene::IncreaseJudgement(){
	judgementIncreaseTime += DELTA_TIME_;
	for (uint8_t i = 0u;i < static_cast<uint8_t>(NoteJudgement::Selection::Size);i++) {

		float_t t = SingleCalculation::InverseLerp(judgementInformation_[i].increaseStartTime, judgementInformation_[i].increaseEndTime, judgementIncreaseTime);
		t = std::clamp(t, 0.0f, 1.0f);

		//スキップ
		if (input_->IsTriggerKey(DIK_RETURN)) {
			t = 1.0f;
		}

		//各桁の値を求めていく
		uint32_t value = static_cast<uint32_t>(static_cast<float_t>(judgementInformation_[i].value * t));

		for (uint8_t j = 0u;j < ONE_THOUSAND_DIGIT_;j++) {
			//各桁の数字を求める
			uint8_t digit = value % 10;
			judgementInformation_[i].valueDigitInformationArray_[j].textureHandle = numberTextureHandle_[static_cast<uint16_t>(digit)];
			//10で割っていく
			value /= 10;
		}

	}

	//コンボの増加へ
	if (judgementIncreaseTime >= judgementInformation_[static_cast<uint8_t>(NoteJudgement::Selection::Miss)].increaseEndTime) {
		currentState_ = ResultSceneState::IncreaseMaxCombo;
	}
}

void ResultScene::IncreaseMaxCombo(){
	increaseMaxComboTime_ += DELTA_TIME_;
	float_t t = SingleCalculation::InverseLerp(0.0f, MAX_COMBO_INCREASE_TIME_, increaseMaxComboTime_);
	t = std::clamp(t, 0.0f, 1.0f);

	//スキップ
	if (input_->IsTriggerKey(DIK_RETURN)) {
		t = 1.0f;
	}

	//各桁の値を求めていく
	uint32_t value = static_cast<uint32_t>(static_cast<float_t>(temporaryRecievedRecord_.maxCombo * t));

	for (uint8_t i = 0u; i < ONE_THOUSAND_DIGIT_; i++) {
		//各桁の数字を求める
		uint8_t digit = value % 10;
		maxComboDigit_[i].textureHandle = numberTextureHandle_[static_cast<uint16_t>(digit)];
		//10で割っていく
		value /= 10;
	}

	//ランクへ
	if (t >= 1.0f) {
		currentState_ = ResultSceneState::IncreaseScore;
	}
}

void ResultScene::IncreaseScore(){
	//スコアの上昇
	scoreIncreaseTime_ += DELTA_TIME_;
	float_t t = SingleCalculation::InverseLerp(0.0f, MAX_GAUGE_INCREASE_TIME_, scoreIncreaseTime_);
	t = std::clamp(t, 0.0f, 1.0f);

	//スキップ
	if (input_->IsTriggerKey(DIK_RETURN)) {
		t = 1.0f;
	}

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
		currentState_ = ResultSceneState::MoveRank;
	}
}


void ResultScene::MoveRank() {
	//ランク表示
	rankSprite_->SetInvisible(false);

	//スケールダウンの時間
	rankScaleDownTime_ += DELTA_TIME_;
	float_t t = SingleCalculation::InverseLerp(0.0f, MAX_SCALE_DOWN_TIME_, rankScaleDownTime_);
	t = std::clamp(t, 0.0f, 1.0f);
	float_t easeScale = Easing::EaseInOutQuart(t) * RANK_SCALE_;
	rankSprite_->SetScale({ .x = easeScale,.y = easeScale });

	if (t >= 1.0f) {
		currentState_ = ResultSceneState::Display;
	}
}

void ResultScene::Display() {
	//表示
	decideSprite_->SetInvisible(false);

	//エンターを押したら戻る
	if (input_->IsTriggerKey(DIK_RETURN)) {
		currentState_ = ResultSceneState::Return;
	}
}

void ResultScene::Return() {
	currentState_ = ResultSceneState::Close;
}

void ResultScene::Close() {
	//トランジションを開く
	if (gameSceneManager_->GetTransition()->SetCloseTransition()) {
		isEnd_ = true;
	}
}
