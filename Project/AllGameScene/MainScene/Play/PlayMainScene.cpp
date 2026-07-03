#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Audio.h>
#include <ModelManager.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <MainScene/End/EndMainScene.h>

#include <Note/Long/LongNote.h>

PlayMainScene::PlayMainScene(){
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
}

void PlayMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);

	//楽曲情報を取得
	musicInformation_ = mainScene_->GetGameManager()->GetMusicInformation();

#ifdef _DEBUG
	//譜面データを取得
	musicScoreData_ = mainScene_->GetGameManager()->GetScoreDataManager()->GetSampleMusicScoreData();
#endif
	//ノーマルタップノーツのモデルを読み込む
	uint32_t normalNoteModelHandle = modelManager_->Load("Resources/Model/Sample/Cube","Cube.obj");

	//通常ノーツの生成
	for (uint32_t i = 0u;i < NORMAL_NOTEMAX_SIZE_;i++) {
		std::unique_ptr<NormalTapNote> normalTapNote = std::make_unique<NormalTapNote>();
		//初期化
		normalTapNote->Initialize(normalNoteModelHandle);
		//挿入
		normalTapNoteVector_.push_back(std::move(normalTapNote));
	}
	
	//楽曲の再生
	audio_->Play(musicScoreData_.musicHandle, false);
	musicLength_= audio_->GetAudioLength(musicScoreData_.musicHandle);
}

void PlayMainScene::Update(){
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicScoreData_.musicHandle);

	//プレイ中
	if (isPlay_) {

		//譜面の流れる処理
		NoteFlow(musicScoreData_.upInformation,upLaneCondition);
		NoteFlow(musicScoreData_.downInformation,downLaneCondition);

		//ポーズ処理
		Pause();

		//楽曲を再生し終わったとき
		if (musicLength_ < musicTime_) {
			isPlay_ = false;
		}
	}
	else {
		//楽曲停止
		audio_->Stop(musicScoreData_.musicHandle);
		//終了シーンへ
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
		return;
	}
	

#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::InputFloat("楽曲再生時間", &musicTime_);
	ImGui::InputFloat("楽曲の長さ", &musicLength_);
	ImGui::InputFloat("ポーズ時間", &pauseTime_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#endif // _DEBUG
}

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//通常ノーツの設定
	for(const std::unique_ptr<NormalTapNote>& note : normalTapNoteVector_) {
		if (note->GetIsUsed()) {
			note->DrawObject3D(camera, baseLight);
		}
	}
}

void PlayMainScene::DrawSprite(){
	//再開するときの演出スプライトを描画する
	if (isRestart_) {

	}
}

void PlayMainScene::NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition){
	int32_t closestNoteIndex = -1;
	for (size_t i = 0u; i < noteInformations.size(); i++) {
		NoteInformation& note = noteInformations[i];

		//判定済みは処理せず次へ
		if (note.isJudged && note.isProcessEnd) {
			continue;
		}

		//タップ用
		if (note.type == NoteType::NormalTap) {
			if (note.startMoveTime > musicTime_) {
				break;
			}
			//動きの割合
			note.moveRatio = SingleCalculation::InverseLerp(note.startMoveTime, note.arriveLineTime, musicTime_);
			

			//通常ノーツの設定
			for (uint32_t i = 0u; i < normalTapNoteVector_.size(); i++) {
				//未使用時
				if (!normalTapNoteVector_[i]->GetIsUsed()) {
					//使用中にする
					if (note.moveRatio >= 0.0f ) {
						normalTapNoteVector_[i]->SetIsUsed(true);
					}
				}
				//使用時
				else {
					//比率の割合を設定
					normalTapNoteVector_[i]->SetRatio(note.moveRatio);
					//更新
					normalTapNoteVector_[i]->Update();
				}	
			}
		}
		//タップ系
		if (note.type == NoteType::NormalTap ||
			note.type == NoteType::HiPassLongStart ||
			note.type == NoteType::LowPassLongStart ||
			note.type == NoteType::TranceGate8LongStart ||
			note.type == NoteType::TranceGate16LongStart) {
			//入力されたとき
			if (laneCondition.isHit) {
				//最も近いノーツのインデックスを記録
				closestNoteIndex = static_cast<int32_t>(i);
				break;
			}

			//見過ごし用
			float_t overlookValue = musicTime_ - note.arriveLineTime;
			if (overlookValue >= NoteJudgement::Time::MISS) {
				///ミス
				note.judgement = NoteJudgement::Selection::Miss;
				record_.miss++;
				// 判定が確定したらフラグを立てる
				note.isJudged = true;
				note.isProcessEnd = true;
			}
		}
		//ロング終点
		else if (note.type == NoteType::LongEnd) {
			if (note.moveRatio >= 1.0f) {
				laneCondition.isHitLongNote = false;
				note.isJudged = true;
				note.isProcessEnd = true;
			}
		}
	}

	//近いノーツ判定
	if (closestNoteIndex != -1) {
		//近いノーツの情報を取得
		NoteInformation& closestNote = noteInformations[closestNoteIndex];
		//絶対値版
		float_t absJudgementTime = std::abs(laneCondition.touchTime - closestNote.arriveLineTime);
		//通常タップ専用
		if (closestNote.type == NoteType::NormalTap) {
			//Perfect用
			if (absJudgementTime >= 0.0f &&
				absJudgementTime < NoteJudgement::Time::PERFECT) {
				
				//Perfectの値を増やす
				record_.perfect++;
				closestNote.judgement = NoteJudgement::Selection::Perfect;
				//判定が確定したらフラグを立てる
				closestNote.isJudged = true;
				closestNote.isProcessEnd = true;
				//コンボを増やす
				record_.combo++;
				//パーフェクトのスコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::PERFECT * comboBonusScale_);
			}
			//Great用
			else if (absJudgementTime >= NoteJudgement::Time::PERFECT &&
				absJudgementTime < NoteJudgement::Time::GREAT) {
				//効果音を鳴らす

				record_.great++;
				closestNote.judgement = NoteJudgement::Selection::Great;
				//判定が確定したらフラグを立てる
				closestNote.isJudged = true;
				closestNote.isProcessEnd = true;
				//コンボを増やす
				record_.combo++;
				//グレートのスコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GREAT * comboBonusScale_ );
			}
			//Good用
			else if (absJudgementTime >= NoteJudgement::Time::GREAT &&
				absJudgementTime < NoteJudgement::Time::GOOD) {
				//効果音を鳴らす

				record_.good++;
				closestNote.judgement = NoteJudgement::Selection::Good;
				// 判定が確定したらフラグを立てる
				closestNote.isJudged = true;
				closestNote.isProcessEnd = true;
				//コンボを増やす
				record_.combo++;
				//グッドのスコアを加算
				record_.score += static_cast<uint32_t>(NoteJudgement::BasicScore::GOOD * comboBonusScale_);
			}
			//Miss用
			else if (absJudgementTime >= NoteJudgement::Time::GOOD &&
				absJudgementTime < NoteJudgement::Time::MISS) {
				record_.miss++;
				closestNote.judgement = NoteJudgement::Selection::Miss;
				// 判定が確定したらフラグを立てる
				closestNote.isJudged = true;
				closestNote.isProcessEnd = true;
				//コンボを0に戻す
				record_.combo = 0u;
				comboBonusScale_ = INITIAL_COMBO_BONUS_SCALE_;
			}
		}
	}
}

void PlayMainScene::Pause(){
	if (isPause_) {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			isRestart_ = true;
		}
		//再開処理
		if (isRestart_) {
			Restart();
		}
	} 
	else {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//ESCAPEでポーズ
			audio_->Stop(musicScoreData_.musicHandle);
			//ポーズ時間の設定
			pauseTime_ = PAUSE_TIME_;
			isPause_ = true;
		}
	}
}

void PlayMainScene::Restart(){
	//時間変化
	pauseTime_ -= DELTA_TIME_;

	//0になったら再開
	if (pauseTime_ <= 0.0f) {
		//解除
		isPause_ = false;
		isRestart_ = false;
		//再開
		audio_->Resume(musicScoreData_.musicHandle);
	}
}