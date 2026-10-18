#include "TestScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include <Audio.h>
#include <ModelManager.h>
#include <TextureManager.h>
#include <GameSceneManager.h>

TestScene::TestScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void TestScene::Initialize(){
	
	//譜面データを取得
	/*musicScoreData_ = mainScene_->GetScoreData();
	difficulty_ = mainScene_->GetDifficulty();
	musicHandle_ = mainScene_->GetMusicHandle();
	jacketTextureHandle_ = mainScene_->GetjacketTextureHandle();*/

	//ノーマルタップノーツのモデルを読み込む
	uint32_t normalNoteModelHandle = modelManager_->Load("Resources/Model/Sample/Cube", "Cube.obj");

#pragma region ノーツの生成

	//通常ノーツの生成
	for (uint8_t i = 0u; i < NORMAL_NOTE_MAX_SIZE_; i++) {
		std::unique_ptr<NormalTapNote> normalTapNote = std::make_unique<NormalTapNote>();
		//初期化
		normalTapNote->Initialize(normalNoteModelHandle);
		normalTapNote->SetInitialPositionX(INITIAL_POSITION_.x);
		normalTapNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		normalTapNoteArray_[i] = std::move(normalTapNote);
	}

	//ハイパスロングノーツの生成
	for (uint8_t i = 0u; i < HI_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		std::unique_ptr<HighPassLongNote> hiPassLongNote = std::make_unique<HighPassLongNote>();
		//初期化
		hiPassLongNote->Initialize(normalNoteModelHandle);
		hiPassLongNote->SetInitialPositionX(INITIAL_POSITION_.x);
		hiPassLongNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		highPassLongNoteArray_[i] = std::move(hiPassLongNote);
	}

	//ローパスロングノーツの生成
	for (uint8_t i = 0u; i < LOW_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		std::unique_ptr<LowPassLongNote> lowPassLongNote = std::make_unique<LowPassLongNote>();
		//初期化
		lowPassLongNote->Initialize(normalNoteModelHandle);
		lowPassLongNote->SetInitialPositionX(INITIAL_POSITION_.x);
		lowPassLongNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		lowPassLongNoteArray_[i] = std::move(lowPassLongNote);
	}

	//トランスゲート8分ロングノーツの生成
	for (uint8_t i = 0u; i < TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_; i++) {
		std::unique_ptr<TranceGateEighthLongNote> tranceGateEighthLongNote = std::make_unique<TranceGateEighthLongNote>();
		//初期化
		tranceGateEighthLongNote->Initialize(normalNoteModelHandle);
		tranceGateEighthLongNote->SetInitialPositionX(INITIAL_POSITION_.x);
		tranceGateEighthLongNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		transGateEighthLongNoteArray_[i] = std::move(tranceGateEighthLongNote);
	}

	//トランスゲート16分ロングノーツの生成
	for (uint8_t i = 0u; i < TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_; i++) {
		std::unique_ptr<TranceGateSixteenthLongNote> tranceGateSixteenthLongNote = std::make_unique<TranceGateSixteenthLongNote>();
		//初期化
		tranceGateSixteenthLongNote->Initialize(normalNoteModelHandle);
		tranceGateSixteenthLongNote->SetInitialPositionX(INITIAL_POSITION_.x);
		tranceGateSixteenthLongNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		transGateSixteenthLongNoteArray_[i] = std::move(tranceGateSixteenthLongNote);
	}

#pragma endregion	

	//判定線のモデルを生成
	judgementLine_ = std::make_unique<JudgementLine>();
	judgementLine_->Initialize(normalNoteModelHandle);

	//譜面管理クラスより譜面データをいただく
	scoreData_ = gameSceneManager_->GetScoreDataManager()->GetExistScoreData();



	//カメラ
	camera_.Initialize();
	camera_.translate = { .x = 0.0f, .y = 5.0f, .z = -100.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();
}

void TestScene::Update(){

	

#ifdef _DEBUG

	ImGui::Begin("譜面エディタ");
	std::vector<std::string> loaedMusicName = {};
	for (size_t i = 0; i < scoreData_.size(); i++) {
		loaedMusicName.push_back(scoreData_[i].musicName);
	}
	if (!scoreData_.empty()) {
		
		//コンボを閉じている時に表示される文字列
		const char* preview = scoreData_[currentSceneNumber_].musicName.c_str();

		if (ImGui::BeginCombo("楽曲", preview)) {
			for (size_t i = 0; i < scoreData_.size(); i++) {
				const bool isSelected = (currentSceneNumber_ == i);

				if (ImGui::Selectable(scoreData_[i].musicName.c_str(), isSelected)) {
					currentSceneNumber_ = i;
				}
				//選択中の項目にフォーカスを当てる
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::Text("楽曲データがありません");
	}
	
	//再生時
	if (!isStop_ && isPlay_) {
		if (ImGui::Button("停止")) {
			//停止状態にする
			isStop_ = true;
			isPlay_ = false;
		}
	}
	
	//停止時
	if (isStop_ && !isPlay_) {
		ImGui::SliderFloat("開始時間", &startTime_, MUSIC_START_TIME_, MUSIC_END_TIME_);

		if (ImGui::Button("再生")) {
			
			//開始状態にする
			isStop_ = false;
			isPlay_ = true;
		}

		if (ImGui::Button("再読み込み")) {
			gameSceneManager_->GetScoreDataManager()->Reload();

		}

	}
	
	//本番で確認
	if (ImGui::Button("本番確認")) {

		gameSceneManager_->ChangeScene("Main");
		return;
	}

	ImGui::End();
#endif // _DEBUG

	//更新
	camera_.Update();
	directionalLight_.Update();
}

void TestScene::DrawObject3D(){

}

void TestScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void TestScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void TestScene::DrawSprite(){

}


