#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Audio.h>
#include <ModelManager.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <MainScene/End/EndMainScene.h>
#include <Windows/WindowsSetup.h>
#include <TextureManager.h>
#include <Note/Long/HighPass/HighPassLongNote.h>

PlayMainScene::PlayMainScene() {
	//ウィンドウの設定
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void PlayMainScene::Initialize() {
	//メインシーンの空チェック
	assert(mainScene_);

	//譜面データを取得
	musicScoreData_ = mainScene_->GetScoreData();
	//ノーマルタップノーツのモデルを読み込む
	uint32_t normalNoteModelHandle = modelManager_->Load("Resources/Model/Sample/Cube", "Cube.obj");

	//判定線のモデルを生成
	judgementLine_ = std::make_unique<JudgementLine>();
	judgementLine_->Initialize(normalNoteModelHandle);

#pragma region ノーツの生成

	//通常ノーツの生成
	for (uint8_t i = 0u;i < NORMAL_NOTE_MAX_SIZE_;i++) {
		std::unique_ptr<NormalTapNote> normalTapNote = std::make_unique<NormalTapNote>();
		//初期化
		normalTapNote->Initialize(normalNoteModelHandle);
		normalTapNote->SetInitialPositionX(INITIAL_POSITION_X_);
		normalTapNote->SetJudgmentPositionX(judgementLine_->GetJudgementPositionX());
		//挿入
		normalTapNoteArray_[i] = std::move(normalTapNote);
	}

	//ハイパスロングノーツの生成
	for (uint8_t i = 0u;i < HI_PASS_LONG_NOTE_MAX_SIZE_;i++) {
		std::unique_ptr<HighPassLongNote> hiPassLongNote = std::make_unique<HighPassLongNote>();
		//初期化
		hiPassLongNote->Initialize(normalNoteModelHandle);
		hiPassLongNote->SetInitialPositionX(INITIAL_POSITION_X_);
		hiPassLongNote->SetJudgmentPositionX(judgementLine_->GetJudgementPositionX());
		//挿入
		highPassLongNoteArray_[i] = std::move(hiPassLongNote);
	}

	//ローパスロングノーツの生成
	for (uint8_t i = 0u;i < LOW_PASS_LONG_NOTE_MAX_SIZE_;i++) {
		std::unique_ptr<LowPassLongNote> lowPassLongNote = std::make_unique<LowPassLongNote>();
		//初期化
		lowPassLongNote->Initialize(normalNoteModelHandle);
		lowPassLongNote->SetInitialPositionX(INITIAL_POSITION_X_);
		lowPassLongNote->SetJudgmentPositionX(judgementLine_->GetJudgementPositionX());
		//挿入
		lowPassLongNoteArray_[i] = std::move(lowPassLongNote);
	}

#pragma endregion	

	//ロングノーツサンプル
	longNoteSmaple_ = std::make_unique<HighPassLongNote>();
	longNoteSmaple_->Initialize(normalNoteModelHandle);
#pragma region ポーズ
	//ポーズ
	uint32_t blackTextureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");
	//カウントダウン用のテクスチャハンドルを読み込む
	std::vector<uint32_t>numberTextureHandleVector = {};
	//何秒
	uint8_t countNumber = 3u;
	//カウントダウンの読み込み
	for (uint8_t i = 0u;i < countNumber;i++) {
		numberTextureHandleVector.push_back(textureManager_->Load("Resources/Sprite/Number/CountDown/" + std::to_string(i + 1u) + ".png"));
	}
	//ポーズのアセット
	pauseAsset_ = std::make_unique<PauseAsset>();
	pauseAsset_->Initilaize(blackTextureHandle, numberTextureHandleVector);

#pragma endregion

	//楽曲の再生
	audio_->Play(musicScoreData_.musicHandle, false);
	musicLength_ = audio_->GetAudioLength(musicScoreData_.musicHandle);
}

void PlayMainScene::Update() {
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicScoreData_.musicHandle);

	//プレイ中
	if (isPlay_) {

		//入力処理
		//上
		Touch(upLaneCondition, DIK_R, DIK_I);
		//下
		Touch(downLaneCondition, DIK_F, DIK_J);

		//譜面の流れる処理
		//上
		NoteFlow(musicScoreData_.upInformation, upLaneCondition);
		//下
		NoteFlow(musicScoreData_.downInformation, downLaneCondition);

		//ポーズ処理
		Pause();

		//楽曲を再生し終わったとき
		if (musicLength_ < musicTime_) {
			isPlay_ = false;
		}
	} else {
		//楽曲停止
		audio_->Stop(musicScoreData_.musicHandle);
		//終了シーンへ
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
		return;
	}
	//ロングノーツのサンプルの更新
	longNoteSmaple_->Update();
	//判定線の更新
	judgementLine_->Update();

#ifdef _DEBUG
	ImGui::Begin("プレイシーン");
	ImGui::Checkbox("上ボタン", &upLaneCondition.isHit);
	ImGui::Checkbox("下ボタン", &downLaneCondition.isHit);

	ImGui::InputFloat("楽曲再生時間", &musicTime_);
	ImGui::InputFloat("楽曲の長さ", &musicLength_);
	if (ImGui::TreeNode("判定")) {
		int32_t perfect = static_cast<int32_t>(record_.perfect);
		int32_t great = static_cast<int32_t>(record_.great);
		int32_t good = static_cast<int32_t>(record_.good);
		int32_t miss = static_cast<int32_t>(record_.miss);
		int32_t combo = static_cast<int32_t>(record_.combo);
		int32_t maxCombo = static_cast<int32_t>(record_.maxCombo);
		int32_t score = static_cast<int32_t>(record_.score);

		ImGui::InputInt("Perfect", &perfect);
		ImGui::InputInt("Great", &great);
		ImGui::InputInt("Good", &good);
		ImGui::InputInt("Miss", &miss);
		ImGui::InputInt("Combo", &combo);
		ImGui::InputInt("MaxCombo", &maxCombo);
		ImGui::InputInt("Score", &score);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("上レーン")) {
		for (uint32_t i = 0u;i < NORMAL_NOTE_MAX_SIZE_;i++) {
			bool isUsed = normalTapNoteArray_[i]->GetIsUsed();
			if (isUsed) {
				float_t startTime = normalTapNoteArray_[i]->GetStartMoveTime();
				float_t arriveTime = normalTapNoteArray_[i]->GetArriveLineTime();
				ImGui::Checkbox("使用中", &isUsed);
				ImGui::InputFloat("移動開始時間", &startTime);
				ImGui::InputFloat("到達時間", &arriveTime);
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#endif // _DEBUG
}

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight) {
	//通常ノーツの設定
	for (uint8_t i = 0u; i < NORMAL_NOTE_MAX_SIZE_; i++) {
		if (normalTapNoteArray_[i]->GetIsUsed()) {
			normalTapNoteArray_[i]->DrawObject3D(camera, baseLight);
		}
	}

	for (uint8_t i = 0u; i < HI_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		if (highPassLongNoteArray_[i]->GetIsUsed()) {
			highPassLongNoteArray_[i]->DrawObject3D(camera, baseLight);
		}
	}

	for (uint8_t i = 0u; i < LOW_PASS_LONG_NOTE_MAX_SIZE_; i++) {
		if (lowPassLongNoteArray_[i]->GetIsUsed()) {
			lowPassLongNoteArray_[i]->DrawObject3D(camera, baseLight);
		}
	}

	//判定線の描画
	judgementLine_->Draw(camera, baseLight);

}

void PlayMainScene::DrawSprite() {
	//ポーズ中
	if (isPause_) {
		//アセットの描画
		pauseAsset_->Draw();
	}
}

void PlayMainScene::NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition) {
	int32_t closestNoteIndex = -1;
	for (size_t i = 0u; i < noteInformations.size(); i++) {
		NoteInformation& note = noteInformations[i];

		//判定済みは処理せず次へ
		if (note.isJudged) {
			continue;
		}
		//まだ動き始める時間になっていないので処理をしない
		if (note.startMoveTime > musicTime_) {
			break;
		}

		//通常タップ
		if (note.type == NoteType::NormalTap) {
			if (!note.isAssigned) {
				//通常ノーツの設定
				for (uint8_t j = 0u; j < NORMAL_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!normalTapNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						normalTapNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[note.place]);
						//開始時間を設定
						normalTapNoteArray_[j]->SetStartMoveTime(note.startMoveTime);
						//到着時間を設定
						normalTapNoteArray_[j]->SetArriveLineTime(note.arriveLineTime);
						//使用中に設定
						normalTapNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						normalTapNoteArray_[j]->SetPoolIndex(j);
						note.poolIndex = j;
						//割り当て済みにする
						note.isAssigned = true;
						break;
					}
				}
			}

		}
		//ハイパス
		else if (note.type == NoteType::HiPassLongStart) {
			if (!note.isAssigned) {
				//通常ノーツの設定
				for (uint8_t j = 0u; j < HI_PASS_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!highPassLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						highPassLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[note.place]);
						//開始時間を設定
						highPassLongNoteArray_[j]->SetStartMoveTime(note.startMoveTime);
						//到着時間を設定
						highPassLongNoteArray_[j]->SetArriveLineTime(note.arriveLineTime);
						//使用中に設定
						highPassLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						highPassLongNoteArray_[j]->SetPoolIndex(j);
						note.poolIndex = j;
						//割り当て済みにする
						note.isAssigned = true;
						break;
					}
				}
			}
		}
		//ローパス
		else if (note.type == NoteType::LowPassLongStart) {
			if (!note.isAssigned) {
				//通常ノーツの設定
				for (uint8_t j = 0u; j < LOW_PASS_LONG_NOTE_MAX_SIZE_; j++) {
					//未使用時
					if (!lowPassLongNoteArray_[j]->GetIsUsed()) {
						//Y座標の設定
						lowPassLongNoteArray_[j]->SetLanePositionY(LANE_POSITION_Y_[note.place]);
						//開始時間を設定
						lowPassLongNoteArray_[j]->SetStartMoveTime(note.startMoveTime);
						//到着時間を設定
						lowPassLongNoteArray_[j]->SetArriveLineTime(note.arriveLineTime);
						//使用中に設定
						lowPassLongNoteArray_[j]->SetIsUsed(true);
						//インデックスを保存
						lowPassLongNoteArray_[j]->SetPoolIndex(j);
						note.poolIndex = j;
						//割り当て済みにする
						note.isAssigned = true;
						break;
					}
				}
			}
		}

		//タップ系
		if (note.type == NoteType::NormalTap) {
			//入力されたとき
			if (laneCondition.isHit) {
				//最も近いノーツのインデックスを記録
				closestNoteIndex = static_cast<int32_t>(i);
				break;
			}

			//見過ごし用
			float_t overlookValue = musicTime_ - note.arriveLineTime;
			if (overlookValue >= NoteJudgement::Time::MISS) {
				//ミス
				note.judgement = NoteJudgement::Selection::Miss;
				record_.miss++;
				//判定が確定したらフラグを立てる
				note.isJudged = true;
			}
		}
		//ロング終点
		else if (note.type == NoteType::LongEnd) {
			note.moveRatio = SingleCalculation::InverseLerp(note.startMoveTime, note.arriveLineTime, musicTime_);
			if (note.moveRatio >= 1.0f) {
				laneCondition.isHitLongNote = false;
				note.isJudged = true;
			}
		}
	}
	//判定
	Judge(noteInformations, laneCondition, closestNoteIndex);

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

			size_t startIndex = 0u;
			for (size_t j = 0u; j < noteInformations.size(); j++) {
				if (noteInformations[j].type == NoteType::HiPassLongStart) {
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
					highPassLongNoteArray_[i]->SetEndRatio(currentRatio);
					break;
				}
			}
			//楽曲時間を設定
			highPassLongNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			highPassLongNoteArray_[i]->Update();
		}
	}
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
}

void PlayMainScene::Judge(std::vector<NoteInformation>& noteInformation, LaneCondition& laneCondition, const int32_t& closestNoteIndex) {
	//近いノーツを判定
	if (closestNoteIndex != -1) {
		//対象のノーツ
		NoteInformation& targetNote = noteInformation[closestNoteIndex];
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
	}
}

void PlayMainScene::Pause() {
	//ポーズ中
	if (isPause_) {

		//再開処理
		if (pauseAsset_->GetIsEnd()) {
			//音量をもとに戻す
			audio_->ChangeVolume(musicScoreData_.musicHandle, 1.0f);
			//再生する
			audio_->Resume(musicScoreData_.musicHandle);
			//ポーズ解除
			isPause_ = false;
			//初期状態に戻す
			pauseAsset_->SetIsEnd(false);
		}

		//ESCAPEで再開
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//カウント開始
			pauseAsset_->SetIsCountDown(true);
		}
		//アセットの更新
		pauseAsset_->Update();
	} else {
		//ESCAPEかウィンドウの移動中の時
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			Stop();
		}
		//ウィンドウのタイトルバーに触れた時
		windowsSetup_->SetOnEnterSizeMoveCallback([this]() {
			Stop();
			}
		);
	}
}

void PlayMainScene::Stop() {
	//ESCAPEでポーズ
	audio_->Stop(musicScoreData_.musicHandle);
	//音量も0にする
	audio_->ChangeVolume(musicScoreData_.musicHandle, 0.0f);
	//止めた時間を記録
	stopTime_ = musicTime_;
	//ポーズ中にする
	isPause_ = true;
}

void PlayMainScene::Touch(LaneCondition& laneCondition, const uint8_t& inputLeft, const uint8_t inputRight) {
	if (input_->IsTriggerKey(inputLeft) || input_->IsTriggerKey(inputRight)) {
		laneCondition.isHit = true;
		laneCondition.touchTime = musicTime_;
	} else {
		laneCondition.isHit = false;
	}

	if (input_->IsPushKey(inputLeft) || input_->IsPushKey(inputRight)) {
		laneCondition.isHold = true;
	} else {
		laneCondition.isHold = false;
	}
}
