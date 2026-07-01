#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Audio.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <MainScene/End/EndMainScene.h>
#include <Note/NormalTap/NormalTapNote.h>
#include <Note/Long/LongNote.h>

PlayMainScene::PlayMainScene(){
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();

}

void PlayMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);

	//楽曲情報を取得
	musicInformation_ = mainScene_->GetGameManager()->GetMusicInformation();

#ifdef _DEBUG
	//譜面データを取得
	musicInformation = mainScene_->GetGameManager()->GetScoreDataManager()->GetSampleMusicScoreData();
#endif

	//楽曲の再生
	audio_->Play(musicInformation.handle, false);
	musicLength_= audio_->GetAudioLength(musicInformation.handle);
}

void PlayMainScene::Update(){
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicInformation.handle);

	//プレイ中
	if (isPlay_) {

		//譜面の流れる処理
		NoteFlowProcess();
		//ポーズ処理
		PauseProcess();

		//楽曲を再生し終わったとき
		if (musicLength_ > musicTime_) {
			isPlay_ = false;
		}
	}
	else {
		//楽曲停止
		audio_->Stop(musicInformation.handle);
		//終了シーンへ
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
		return;
	}
	

#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::InputFloat("楽曲再生時間", &musicTime_);
	ImGui::InputFloat("楽曲の長さ", &musicLength_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#endif // _DEBUG
}

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void PlayMainScene::PreDrawPostEffect(){

}

void PlayMainScene::DrawPostEffect(){

}

void PlayMainScene::DrawSprite(){

}

void PlayMainScene::NoteFlowProcess(){
	int32_t ClosestNoteIndex = -1;
	for (size_t i = 0u; i < musicInformation.upInformation.size(); i++) {
		NoteInformation& note = musicInformation.upInformation[i];

		//判定済みは処理しない
		if (note.isJudged && note.isProcessEnd) {
			
			//データが入っていれば削除する
			continue;
		}

		//タップ用
		if (note.TapNote) {
			if (note.startMoveTime > musicTime_) {
				break;
			}
			//動きの割合
			note.moveRatio = SingleCalculation::InverseLerp(note.startMoveTime, note.arriveLineTime, musicTime_);
			//座標の計算
			note.currentPosition.x = SingleCalculation::Lerp(START_POSITION_X_, JUDGEENT_POSITION_[NoteLane::Place::Up].x, note.moveRatio);
			note.currentPosition.y = note.initialPosition.y;
			note.currentPosition.z = 0.0f;
			
			
			//座標の設定

			//線形補間で0～1(少し過ぎたぐらい)の時だけ表示させる
			if (note.moveRatio >= 0.0f &&
				note.moveRatio <= 1.2f) {
				note.TapNote->SetActorHiddenInGame(false);
			} else {
				note.TapNote->SetActorHiddenInGame(true);
			}
		}
		//タップ系
		if (note.NoteSelection == E_NoteSelection::NormalTapNote ||
			note.NoteSelection == E_NoteSelection::LongNoteStart ) {
			//入力されたとき
			if (laneCondition.isHit) {
				//最も近いノーツのインデックスを記録
				ClosestNoteIndex = i;
				break;
			}

			//見過ごし用
			float OverlookValue = MusicTime_ - note.JudgementArrivalTime;
			if (OverlookValue >= MISS_TAP_) {
				if (note.NoteSelection != E_NoteSelection::RecoverNote) {
					laneCondition.judgementResult_.miss++;
					if (!bJudgementShownThisFrame && IsValid(JudgementUIInstance)) {
						JudgementUIInstance->ShowJudgement(E_NoteJudgementSelection::Miss);
						bJudgementShownThisFrame = true;
					}
					note.Judgement = E_NoteJudgementSelection::Miss;
					//コンボを0に戻す
					allResult_.Combo_ = 0u;
					//追加
					if (IsValid(JudgementUIInstance)) {
						JudgementUIInstance->ResetCenterCombo();
					}
					//通常ダメージ
					PlayerCondition_->MissDamage();
				} else {
					note.Judgement = E_NoteJudgementSelection::Ignore;
				}
				// 判定が確定したらフラグを立てる
				note.IsJudged = true;
				note.IsProcessEnd = true;
			}
		}
		//ロング終点
		else if (note.NoteSelection == E_NoteSelection::LongNoteEnd) {
			if (note.moveRatio >= 1.0f) {
				laneCondition.isHitLongNote = false;
				note.isProcessEnd = true;
				note.isJudged = true;
			}
		}
	}

	//近いノーツ判定
	if (ClosestNoteIndex != -1) {
		//近いノーツの情報を取得
		NoteInformation& ClosestNote = musicInformation.upInformation[ClosestNoteIndex];
		//絶対値版
		float AbsJudgementTime = std::abs(laneCondition.touchTime - ClosestNote.arriveLineTime);
		//通常タップ専用
		if (ClosestNote.NoteSelection == E_NoteSelection::NormalTapNote) {
			//Perfect用
			if (AbsJudgementTime >= 0.0f &&
				AbsJudgementTime < NoteJudgement::Time::PERFECT_TAP) {
				//効果音を鳴らす

				laneCondition.judgementResult_.perfect++;
				ClosestNote.judgement = NoteJudgement::Selection::Perfect;
				//判定が確定したらフラグを立てる
				ClosestNote.isJudged = true;
				ClosestNote.isProcessEnd = true;
				//コンボを増やす
				allResult_.Combo_++;

				//パーフェクトのスコアを加算
				Score_ += PERFECT_INCREASE_SCORE_ * ComboBonusScale_;

				laneCondition.isAttack = true;
				laneCondition.IsReleaseJudgement = E_NoteJudgementSelection::Perfect;

			}
			//Great用
			else if (AbsJudgementTime >= NoteJudgement::Time::PERFECT_TAP &&
				AbsJudgementTime < NoteJudgement::Time::GREAT_TAP) {
				//効果音を鳴らす

				laneCondition.judgementResult_.great++;
				ClosestNote.judgement = NoteJudgement::Selection::Great;
				//判定が確定したらフラグを立てる
				ClosestNote.isJudged = true;
				ClosestNote.isProcessEnd = true;
				//コンボを増やす
				allResult_.Combo_++;

				Score_ += GREAT_INCREASE_SCORE_ * ComboBonusScale_ ;
				laneCondition.isAttack = true;
				laneCondition.IsReleaseJudgement = NoteJudgement::Selection::Great;
			}
			//Good用
			else if (AbsJudgementTime >= NoteJudgement::Time::GREAT_TAP &&
				AbsJudgementTime < NoteJudgement::Time::GOOD_TAP) {
				//効果音を鳴らす

				laneCondition.judgementResult_.good++;
				ClosestNote.judgement = NoteJudgement::Selection::Good;
				// 判定が確定したらフラグを立てる
				ClosestNote.isJudged = true;
				ClosestNote.isProcessEnd = true;
				//コンボを増やす
				allResult_.Combo_++;
				//グレートのスコアを加算
				Score_ += GOOD_INCREASE_SCORE_ * ComboBonusScale_;
			}
			//Miss用
			else if (AbsJudgementTime >= NoteJudgement::Time::GOOD_TAP &&
				AbsJudgementTime < NoteJudgement::Time::MISS_TAP) {
				laneCondition.judgementResult_.miss++;
				ClosestNote.judgement = NoteJudgement::Selection::Miss;
				// 判定が確定したらフラグを立てる
				ClosestNote.isJudged = true;
				ClosestNote.isProcessEnd = true;
				//コンボを0に戻す
				allResult_.Combo_ = 0u;

			}
		}
	}
}

void PlayMainScene::PauseProcess(){
	if (isPause_) {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//解除
			isPause_ = false;
		}
	} else {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//ESCAPEでポーズ
			audio_->Stop(musicInformation.handle);
			isPause_ = true;
		}
	}
}

