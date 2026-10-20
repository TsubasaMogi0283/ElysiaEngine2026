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
	camera_.translate = { .x = 0.0f, .y = 5.0f, .z = -70.0f };
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//すぐに開ける
	gameSceneManager_->GetTransition()->ShortkyOpen();

	std::string musicName = scoreData_[currentMusicNumber_].musicName;
	std::string difficultyName = difficultyVector_[currentDifficultyNumber_];
	musicData_ = gameSceneManager_->GetScoreDataManager()->GetMusicScoreData(musicName, difficultyName);
	existMusicScoreData_ = gameSceneManager_->GetScoreDataManager()->GetExistScoreData()[currentMusicNumber_];

	//生成
	GenerateNotes();
}

void TestScene::Update(){

	if (!isStop_ && isPlay_) {
		musicTime_ = audio_->GetPlayCurrentTime(existMusicScoreData_.musicHandle);

		//入力処理
		//上
		Touch(upLaneCondition, DIK_R, DIK_I);
		//下
		Touch(downLaneCondition, DIK_F, DIK_J);

		//譜面の流れる処理
		//上
		NoteFlow(musicData_.upInformation, upLaneCondition);
		//下
		NoteFlow(musicData_.downInformation, downLaneCondition);

	}
	
#ifdef _DEBUG

	ImGui::Begin("譜面エディタ");
	ImGui::InputFloat("再生時間", &musicTime_);

	//再生時
	if (!isStop_ && isPlay_) {
		if (ImGui::Button("停止")) {
			//停止状態にする
			isStop_ = true;
			isPlay_ = false;
			isResume_ = true;

			//音を停止
			audio_->Stop(existMusicScoreData_.musicHandle);
		}
	}
	
	//停止時
	if (isStop_ && !isPlay_) {
		if (ImGui::Button("再生")) {
			//再生
			if (isResume_) {
				audio_->Resume(existMusicScoreData_.musicHandle);
				isResume_ = false;
			}
			else {
				audio_->Play(existMusicScoreData_.musicHandle, startTime_);
			}
			

			//開始状態にする
			isStop_ = false;
			isPlay_ = true;
		}

		if (ImGui::TreeNode("再設定")) {
			ImGui::SliderFloat("開始時間", &startTime_, MUSIC_START_TIME_, MUSIC_END_TIME_);
			ImGui::SliderFloat("ハイスピ", &hiSpeed_,0.0f,3.0f);

			//楽曲が入っている時しか処理しないようにする
			//入っていない状態でやっても意味ないからね
			if (!scoreData_.empty()) {

				//コンボを閉じている時に表示される文字列
				const char* musicPreview = scoreData_[currentMusicNumber_].musicName.c_str();
				if (ImGui::BeginCombo("楽曲", musicPreview)) {
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


				//コンボを閉じている時に表示される文字列
				const char* difficultyPreview = difficultyVector_[currentDifficultyNumber_].c_str();
				if (ImGui::BeginCombo("楽曲", difficultyPreview)) {
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
					
					isResume_ = false;
					//ノーツ生成
					GenerateNotes();
				}
			}
			else {
				ImGui::Text("楽曲データがありません");
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
	float_t startTime = START_OFFSET_TIME_+ hiSpeed_;
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

void TestScene::NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition){
	int32_t closestNoteIndex = -1;
	for (size_t i = 0u; i < noteInformations.size(); i++) {
		NoteInformation& startNote = noteInformations[i];

		//判定済みは処理せず次へ
		if (startNote.isJudged) {
			continue;
		}
		//まだ動き始める時間になっていないので処理をしない
		if (startNote.startMoveTime > musicTime_) {
			break;
		}

		if (!startNote.isAssigned) {
			//ノーツの種類によってオブジェクトプールから割り当てる
			switch (startNote.type) {
			case NoteType::NormalTap:
				//通常ノーツの設定
				for (uint8_t j = 0u; j < NORMAL_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!normalTapNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						normalTapNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[startNote.place]);
						//開始時間を設定
						normalTapNoteArray_[j]->SetStartMoveTime(startNote.startMoveTime);
						//到着時間を設定
						normalTapNoteArray_[j]->SetArriveLineTime(startNote.arriveLineTime);
						//使用中に設定
						normalTapNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						normalTapNoteArray_[j]->SetPoolIndex(j);
						startNote.poolIndex = j;
						//割り当て済みにする
						startNote.isAssigned = true;
						break;
					}
				}
				break;

			case NoteType::HiPassLongStart:
				for (uint8_t j = 0u; j < HI_PASS_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!highPassLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						highPassLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[startNote.place]);
						//開始時間を設定
						highPassLongNoteArray_[j]->SetStartMoveTime(startNote.startMoveTime);
						//到着時間を設定
						highPassLongNoteArray_[j]->SetArriveLineTime(startNote.arriveLineTime);
						//使用中に設定
						highPassLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						highPassLongNoteArray_[j]->SetPoolIndex(j);
						//終点ノーツを探す
						for (size_t k = i; k < noteInformations.size(); k++) {
							if (noteInformations[k].type == NoteType::LongEnd) {
								//移動開始時間と到着時間を設定
								startNote.endStartMoveTime_ = noteInformations[k].startMoveTime;
								startNote.endArriveMoveTime_ = noteInformations[k].arriveLineTime;
								highPassLongNoteArray_[j]->SetEndStartMoveTime(startNote.endStartMoveTime_);
								highPassLongNoteArray_[j]->SetEndArriveLineTime(startNote.endArriveMoveTime_);
								break;
							}
						}
						//オブジェクトプールを記録する
						startNote.poolIndex = j;
						//割り当て済みにする
						startNote.isAssigned = true;
						break;
					}
				}
				break;

			case NoteType::LowPassLongStart:
				for (uint8_t j = 0u; j < LOW_PASS_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!lowPassLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						lowPassLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[startNote.place]);
						//開始時間を設定
						lowPassLongNoteArray_[j]->SetStartMoveTime(startNote.startMoveTime);
						//到着時間を設定
						lowPassLongNoteArray_[j]->SetArriveLineTime(startNote.arriveLineTime);
						//使用中に設定
						lowPassLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						lowPassLongNoteArray_[j]->SetPoolIndex(j);
						startNote.poolIndex = j;
						//割り当て済みにする
						startNote.isAssigned = true;
						break;
					}
				}
				break;

			case NoteType::TranceGate8LongStart:
				for (uint8_t j = 0u; j < TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!transGateEighthLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						transGateEighthLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[startNote.place]);
						//開始時間を設定
						transGateEighthLongNoteArray_[j]->SetStartMoveTime(startNote.startMoveTime);
						//到着時間を設定
						transGateEighthLongNoteArray_[j]->SetArriveLineTime(startNote.arriveLineTime);
						//使用中に設定
						transGateEighthLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						transGateEighthLongNoteArray_[j]->SetPoolIndex(j);
						startNote.poolIndex = j;
						//割り当て済みにする
						startNote.isAssigned = true;
						break;
					}
				}
				break;

			case NoteType::TranceGate16LongStart:
				for (uint8_t j = 0u; j < TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!transGateSixteenthLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						transGateSixteenthLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[startNote.place]);
						//開始時間を設定
						transGateSixteenthLongNoteArray_[j]->SetStartMoveTime(startNote.startMoveTime);
						//到着時間を設定
						transGateSixteenthLongNoteArray_[j]->SetArriveLineTime(startNote.arriveLineTime);
						//使用中に設定
						transGateSixteenthLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						transGateSixteenthLongNoteArray_[j]->SetPoolIndex(j);
						startNote.poolIndex = j;
						//割り当て済みにする
						startNote.isAssigned = true;
						break;
					}
				}
				break;
			}
		}
		
		//タップ系
		if (startNote.type == NoteType::NormalTap) {
			//入力されたとき
			if (laneCondition.isHit) {
				//最も近いノーツのインデックスを記録
				closestNoteIndex = static_cast<int32_t>(i);
				break;
			}

			//見過ごし用
			float_t overlookValue = musicTime_ - startNote.arriveLineTime;
			if (overlookValue >= NoteJudgement::Time::MISS) {
				//ミス
				startNote.judgement = NoteJudgement::Selection::Miss;
				record_.miss++;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(startNote.judgement));
				record_.combo = 0u;
				//判定が確定したらフラグを立てる
				startNote.isJudged = true;
			}
		}
		//ロング系
		else if (startNote.type == NoteType::HiPassLongStart) {
			
			//入力されたとき
			if (laneCondition.isHold) {
				//最も近いノーツのインデックスを記録
				closestNoteIndex = static_cast<int32_t>(i);
				break;
			}

			//見過ごし用
			//開始が指定した時間を超えたらミス
			float_t overlookValue = musicTime_ - startNote.arriveLineTime;
			if (overlookValue >= NoteJudgement::Time::MISS) {
				//ミス
				startNote.judgement = NoteJudgement::Selection::Miss;
				record_.miss++;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(startNote.judgement));
				record_.combo = 0u;
				//判定が確定したらフラグを立てる
				startNote.isJudged = true;
				//未使用に戻す
				//また初期値に戻す
				highPassLongNoteArray_[startNote.poolIndex]->SetIsUsed(false);
				highPassLongNoteArray_[startNote.poolIndex]->SetPoolIndex(-1);
				startNote.poolIndex = -1;
			}
		}
	}

	//近いノーツを判定
	if (closestNoteIndex != -1) {
		//対象のノーツ
		NoteInformation& targetNote = noteInformations[closestNoteIndex];
		//判定時間の差分(絶対値)
		float_t absoluteJudgementTime = std::abs(laneCondition.touchTime - targetNote.arriveLineTime);
		//通常タップ専用
		if (targetNote.type == NoteType::NormalTap) {

			//判定の確定
			bool isConfirmJudgement = false;

			//Perfect用
			if (absoluteJudgementTime < NoteJudgement::Time::PERFECT) {

				//判定の設定
				record_.perfect++;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Perfect));
				targetNote.judgement = NoteJudgement::Selection::Perfect;
				isConfirmJudgement = true;

				//コンボを増やす
				record_.combo++;
				//スコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::PERFECT * comboBonusScale_);
			}
			//Great用
			else if (absoluteJudgementTime < NoteJudgement::Time::GREAT) {

				//判定の設定
				record_.great++;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Great));
				targetNote.judgement = NoteJudgement::Selection::Great;
				isConfirmJudgement = true;

				//コンボを増やす
				record_.combo++;
				//スコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GREAT * comboBonusScale_);
			}
			//Good用
			else if (absoluteJudgementTime < NoteJudgement::Time::GOOD) {

				//判定の設定
				record_.good++;
				targetNote.judgement = NoteJudgement::Selection::Good;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Good));
				isConfirmJudgement = true;

				//コンボを増やす
				record_.combo++;
				//スコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GOOD * comboBonusScale_);
			}
			//Miss用
			else if (absoluteJudgementTime < NoteJudgement::Time::MISS) {

				//判定の設定
				record_.miss++;
				targetNote.judgement = NoteJudgement::Selection::Miss;
				//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Miss));
				isConfirmJudgement = true;

				//コンボを0に戻す
				record_.combo = 0u;
				//スコアの倍率を初期化
				comboBonusScale_ = INITIAL_COMBO_BONUS_SCALE_;
			}

			//判定が確定したらフラグを立てる
			if (isConfirmJudgement) {
				targetNote.isJudged = true;
				//また初期値に戻す
				normalTapNoteArray_[targetNote.poolIndex]->SetIsUsed(false);
				normalTapNoteArray_[targetNote.poolIndex]->SetPoolIndex(-1);
				targetNote.poolIndex = -1;
			}
		}
		else if (targetNote.type == NoteType::HiPassLongStart) {
			
			//判定の確定
			bool isConfirmJudgement = false;
			if (!targetNote.isLongStartJugged) {
				//Perfect用
				if (absoluteJudgementTime < NoteJudgement::Time::PERFECT) {

					//判定の設定
					record_.perfect++;
					//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Perfect));
					//コンボを増やす
					record_.combo++;
					//スコアを加算
					record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::PERFECT * comboBonusScale_);
					//始点を判定済みにする
					targetNote.isLongStartJugged = true;

				}
				//Great用
				else if (absoluteJudgementTime < NoteJudgement::Time::GREAT) {

					//判定の設定
					record_.great++;
					//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Great));
					//コンボを増やす
					record_.combo++;
					//スコアを加算
					record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GREAT * comboBonusScale_);
					//始点を判定済みにする
					targetNote.isLongStartJugged = true;
				}
				//Good用
				else if (absoluteJudgementTime < NoteJudgement::Time::GOOD) {

					//判定の設定
					record_.good++;
					//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Good));
					//コンボを増やす
					record_.combo++;
					//スコアを加算
					record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GOOD * comboBonusScale_);
					//始点を判定済みにする
					targetNote.isLongStartJugged = true;
				}
				//Miss用
				else if (absoluteJudgementTime < NoteJudgement::Time::MISS) {

					//判定の設定
					record_.miss++;
					//mainScene_->GetGauge()->SetIncreaseValue(static_cast<size_t>(NoteJudgement::Selection::Miss));
					//コンボを0に戻す
					record_.combo = 0u;
					//スコアの倍率を初期化
					comboBonusScale_ = INITIAL_COMBO_BONUS_SCALE_;
					//始点を判定済みにする
					targetNote.isLongStartJugged = true;
				}
			}


			//ホールド中スコアが増加
			if (laneCondition.isHold&&
				(targetNote.arriveLineTime <= musicTime_&& musicTime_ <= targetNote.endArriveMoveTime_)) {
				laneCondition.isHoldLongNote = true;
				record_.score += LONG_NOTE_SCORE_INCREASE_VALUE_;
			}
			//比率を計算し、1.0f以上になったら判定済みにする
			float_t endRatio = SingleCalculation::InverseLerp(targetNote.endStartMoveTime_, targetNote.endArriveMoveTime_, musicTime_);
			if (endRatio >= 1.0f) {
				isConfirmJudgement = true;
			}

			//判定が確定したらフラグを立てる
			if (isConfirmJudgement) {
				laneCondition.isHoldLongNote = false;
				targetNote.isJudged = true;

				//また初期値に戻す
				highPassLongNoteArray_[targetNote.poolIndex]->SetIsUsed(false);
				highPassLongNoteArray_[targetNote.poolIndex]->SetPoolIndex(-1);
				targetNote.poolIndex = -1;
			}
		}
	}

#pragma region 各ノーツの更新
	//通常ノーツの設定
	for (uint8_t j = 0u; j < NORMAL_NOTE_MAX_SIZE_; j++) {
		//使用時
		if (normalTapNoteArray_[j]->GetIsUsed()) {
			//楽曲時間を設定
			normalTapNoteArray_[j]->SetMusicTime(musicTime_);
			//更新
			normalTapNoteArray_[j]->Update();
		}
	}
	//ハイパスロングノーツの設定
	for (uint8_t i = 0u; i < HI_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		//使用時
		if (highPassLongNoteArray_[i]->GetIsUsed()) {
			//楽曲時間を設定
			highPassLongNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			highPassLongNoteArray_[i]->Update();
		}
	}

	//ToDo:ハイパスをもとに貼り換え
	//ローパスロングノーツの設定
	for (uint8_t i = 0u; i < LOW_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		//使用時
		if (lowPassLongNoteArray_[i]->GetIsUsed()) {
			size_t startIndex = 0u;
			for (size_t j = 0u; j < noteInformations.size(); j++) {
				if (noteInformations[j].type == NoteType::LowPassLongStart) {
					startIndex = j;
				}
			}

			for (size_t j = startIndex; j < noteInformations.size(); j++) {
				//現在の比率を計算
				//そこから座標を求めていく
				//終了地点を見つけたら探すのをやめる。
				if (noteInformations[j].type == NoteType::LongEnd) {
					//現在の比率を計算し設定
					float_t currentRatio = SingleCalculation::InverseLerp(noteInformations[j].startMoveTime, noteInformations[j].arriveLineTime, musicTime_);
					currentRatio = std::clamp(currentRatio, 0.0f, 1.0f);
					lowPassLongNoteArray_[i]->SetEndRatio(currentRatio);
					break;
				}
			}
			//楽曲時間を設定
			lowPassLongNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			lowPassLongNoteArray_[i]->Update();
		}
	}
	//トランスゲート8ノーツの設定
	for (uint8_t i = 0u; i < TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_; i++) {
		//使用時
		if (transGateEighthLongNoteArray_[i]->GetIsUsed()) {
			size_t startIndex = 0u;
			for (size_t j = 0u; j < noteInformations.size(); j++) {
				if (noteInformations[j].type == NoteType::TranceGate8LongStart) {
					startIndex = j;
				}
			}

			for (size_t j = startIndex; j < noteInformations.size(); j++) {
				//現在の比率を計算
				//そこから座標を求めていく
				//終了地点を見つけたら探すのをやめる。
				if (noteInformations[j].type == NoteType::LongEnd) {
					//現在の比率を計算し設定
					float_t currentRatio = SingleCalculation::InverseLerp(noteInformations[j].startMoveTime, noteInformations[j].arriveLineTime, musicTime_);
					currentRatio = std::clamp(currentRatio, 0.0f, 1.0f);
					transGateEighthLongNoteArray_[i]->SetEndRatio(currentRatio);
					break;
				}
			}
			//楽曲時間を設定
			transGateEighthLongNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			transGateEighthLongNoteArray_[i]->Update();
		}
	}
	//トランスゲート16ノーツの設定
	for (uint8_t i = 0u; i < TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_; i++) {
		//使用時
		if (transGateSixteenthLongNoteArray_[i]->GetIsUsed()) {
			size_t startIndex = 0u;
			for (size_t j = 0u; j < noteInformations.size(); j++) {
				if (noteInformations[j].type == NoteType::TranceGate16LongStart) {
					startIndex = j;
				}
			}

			for (size_t j = startIndex; j < noteInformations.size(); j++) {
				//現在の比率を計算
				//そこから座標を求めていく
				//終了地点を見つけたら探すのをやめる。
				if (noteInformations[j].type == NoteType::LongEnd) {
					//現在の比率を計算し設定
					float_t currentRatio = SingleCalculation::InverseLerp(noteInformations[j].startMoveTime, noteInformations[j].arriveLineTime, musicTime_);
					currentRatio = std::clamp(currentRatio, 0.0f, 1.0f);
					transGateSixteenthLongNoteArray_[i]->SetEndRatio(currentRatio);
					break;
				}
			}
			//楽曲時間を設定
			transGateSixteenthLongNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			transGateSixteenthLongNoteArray_[i]->Update();
		}
	}

#pragma endregion
}

void TestScene::Touch(LaneCondition& laneCondition, const uint8_t& inputLeft, const uint8_t inputRight) {

	//トリガー
	if (input_->IsTriggerKey(inputLeft) || input_->IsTriggerKey(inputRight)) {
		laneCondition.isHit = true;
		laneCondition.touchTime = musicTime_;
	} else {
		laneCondition.isHit = false;
	}

	//長押し
	if (input_->IsPushKey(inputLeft) || input_->IsPushKey(inputRight)) {
		laneCondition.isHold = true;
	} else {
		laneCondition.isHold = false;
	}
}