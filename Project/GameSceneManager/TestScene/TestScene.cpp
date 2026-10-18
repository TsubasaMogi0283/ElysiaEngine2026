#include "TestScene.h"

#include <imgui.h>
#include <numbers>
#include <magic_enum/magic_enum.hpp>

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

	for (uint8_t i = 0u;i< static_cast<uint8_t>(Difficulty::Size); i++) {
		std::string_view difficulty= magic_enum::enum_name(static_cast<Difficulty>(i));
		std::string convertedDiffuculty(difficulty);
		difficultyVector_.push_back(convertedDiffuculty);
	}
	

	//カメラ
	camera_.Initialize();
	camera_.translate = { .x = 0.0f, .y = 5.0f, .z = -100.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//すぐに開ける
	gameSceneManager_->GetTransition()->ShortkyOpen();
}

void TestScene::Update(){

	

#ifdef _DEBUG

	ImGui::Begin("譜面エディタ");
	
	
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
		
		if (ImGui::Button("再生")) {
			
			//開始状態にする
			isStop_ = false;
			isPlay_ = true;
		}

		if (ImGui::TreeNode("再設定")) {

			ImGui::SliderFloat("開始時間", &startTime_, MUSIC_START_TIME_, MUSIC_END_TIME_);


			if (!scoreData_.empty()) {

				//コンボを閉じている時に表示される文字列
				const char* preview = scoreData_[currentMusicNumber_].musicName.c_str();

				if (ImGui::BeginCombo("楽曲", preview)) {
					for (size_t i = 0; i < scoreData_.size(); i++) {
						const bool isSelected = (currentMusicNumber_ == i);

						if (ImGui::Selectable(scoreData_[i].musicName.c_str(), isSelected)) {
							currentMusicNumber_ = i;
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


			//コンボを閉じている時に表示される文字列
			const char* preview = difficultyVector_[currentDifficultyNumber_].c_str();

			if (ImGui::BeginCombo("楽曲", preview)) {
				for (size_t i = 0; i < difficultyVector_.size(); i++) {
					const bool isSelected = (currentDifficultyNumber_ == i);

					if (ImGui::Selectable(difficultyVector_[i].c_str(), isSelected)) {
						currentDifficultyNumber_ = i;
					}
					//選択中の項目にフォーカスを当てる
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("再読み込み")) {
				gameSceneManager_->GetScoreDataManager()->Reload();

				//データを持ってくる
				std::string musicName = scoreData_[currentMusicNumber_].musicName;
				std::string difficultyName = difficultyVector_[currentDifficultyNumber_];
				musicData_ = gameSceneManager_->GetScoreDataManager()->GetMusicScoreData(musicName, difficultyName);
				existMusicScoreData_ = gameSceneManager_->GetScoreDataManager()->GetExistScoreData()[currentMusicNumber_];
				//再生
				audio_->Play(existMusicScoreData_.musicHandle, startTime_);

			}

			ImGui::TreePop();
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
	judgementLine_->Update();
	camera_.Update();
	directionalLight_.Update();
}

void TestScene::DrawObject3D(){
	//通常ノーツの設定
	for (uint8_t i = 0u; i < NORMAL_NOTE_MAX_SIZE_; i++) {
		if (normalTapNoteArray_[i]->GetIsUsed()) {
			normalTapNoteArray_[i]->DrawObject3D(camera_, directionalLight_);
		}
	}

	//ハイパスロングノーツの設定
	for (uint8_t i = 0u; i < HI_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		if (highPassLongNoteArray_[i]->GetIsUsed()) {
			highPassLongNoteArray_[i]->DrawObject3D(camera_, directionalLight_);
		}
	}

	//ローパスロングノーツの設定
	for (uint8_t i = 0u; i < LOW_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		if (lowPassLongNoteArray_[i]->GetIsUsed()) {
			lowPassLongNoteArray_[i]->DrawObject3D(camera_, directionalLight_);
		}
	}

	//トランスゲート8分ロングノーツの設定
	for (uint8_t i = 0u; i < TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_; i++) {
		if (transGateEighthLongNoteArray_[i]->GetIsUsed()) {
			transGateEighthLongNoteArray_[i]->DrawObject3D(camera_, directionalLight_);
		}
	}

	//トランスゲート16分ロングノーツの設定
	for (uint8_t i = 0u; i < TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_; i++) {
		if (transGateSixteenthLongNoteArray_[i]->GetIsUsed()) {
			transGateSixteenthLongNoteArray_[i]->DrawObject3D(camera_, directionalLight_);
		}
	}
	//判定の線を取得
	judgementLine_->Draw(camera_, directionalLight_);
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


void TestScene::GenerateNotes() {
	//合計の時間
	float_t totalTime = 0.0f;
	//開始時間の設定
	float_t startTime = START_OFFSET_TIME_ ;
	//ノーツの配置
	for (const NoteBarInformation& bar : musicData_.newNotesData) {
		//1拍の秒数
		float_t beatDuration = 60.0f / bar.bpm;
		//ノーツ間隔(1小節4拍)
		float_t noteInterval = (beatDuration * 4.0f) / bar.notesLane.size();
		//長さ
		size_t length = bar.notesLane.size();

		for (size_t i = 0u; i < bar.notesLane.size(); i++) {
			const NoteLane::Information& note = bar.notesLane[i];

#pragma region 通常タップ
			//上
			if (note.upNote == NoteType::NormalTap) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::NormalTap,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::NormalTap) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::NormalTap,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region ハイパスロングノーツ
			//上
			if (note.upNote == NoteType::HiPassLongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::HiPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::HiPassLongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::HiPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region ローパスロングノーツ
			//上
			if (note.upNote == NoteType::LowPassLongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::LowPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::LowPassLongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::LowPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region トランスゲート8分ロングノーツ

			//上
			if (note.upNote == NoteType::TranceGate8LongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::TranceGate8LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::TranceGate8LongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::TranceGate8LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}


#pragma endregion


#pragma region トランスゲート16分ロングノーツ

			//上
			if (note.upNote == NoteType::TranceGate16LongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::TranceGate16LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::TranceGate16LongStart) {
				//ノーツの数を増やす
				musicData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::TranceGate16LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}


#pragma endregion

#pragma region ロング終点
			//上
			if (note.upNote == NoteType::LongEnd) {
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::LongEnd,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.upInformation.push_back(noteInformation);
			}

			//下
			if (note.downNote == NoteType::LongEnd) {
				//ノーツ情報を設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::LongEnd,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicData_.downInformation.push_back(noteInformation);
			}
#pragma endregion
		}
		// 小節分進める
		totalTime += 4.0f * beatDuration;
	}
}
