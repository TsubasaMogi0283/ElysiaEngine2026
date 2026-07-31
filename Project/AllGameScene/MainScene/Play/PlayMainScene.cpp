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

PlayMainScene::PlayMainScene(){
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

void PlayMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);

	//譜面データを取得
	musicScoreData_ = mainScene_->GetScoreData();
	//ノーマルタップノーツのモデルを読み込む
	uint32_t normalNoteModelHandle = modelManager_->Load("Resources/Model/Sample/Cube","Cube.obj");

	//通常ノーツの生成
	for (uint32_t i = 0u;i < NORMAL_NOTE_MAX_SIZE_;i++) {
		std::unique_ptr<NormalTapNote> normalTapNote = std::make_unique<NormalTapNote>();
		//初期化
		normalTapNote->Initialize(normalNoteModelHandle);
		normalTapNote->SetInitialPositionX(INITIAL_POSITION_X_);
		normalTapNote->SetJudgmentPositionX(JUDGEMENT_POSITION_X_);
		//挿入
		normalTapNoteArray_[i] = std::move(normalTapNote);
	}
	
	//ロングノーツ
	longNoteSmaple_ = std::make_unique<HighPassLongNote>();
	longNoteSmaple_->Initialize(normalNoteModelHandle);

	//判定線のモデルを生成
	judgementLineModel_ = Elysia::Model::Create(normalNoteModelHandle);
	judgementLineWorldTransform_.Initialize();
	judgementLineWorldTransform_.scale.x = 0.1f;
	judgementLineWorldTransform_.scale.y = 10.0f;
	judgementLineWorldTransform_.translate.x = JUDGEMENT_POSITION_X_;
	judgementLineWorldTransform_.translate.y = 5.0f;
	//判定線のマテリアルを生成
	judgementLineMaterial_.Initialize();
	judgementLineMaterial_.color = { .x = 1.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };

	//ポーズ
	uint32_t blackTextureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");
	//カウントダウン用のテクスチャハンドルを読み込む
	std::vector<uint32_t>numberTextureHandleVector = {};
	//何秒
	uint8_t countNumber = 3u;
	//カウントダウンの読み込み
	for (uint8_t i = 0u;i < countNumber;i++) {
		numberTextureHandleVector.push_back(textureManager_->Load("Resources/Sprite/Number/CountDown/" + std::to_string(i + 1u)+".png"));
	}
	//ポーズのアセット
	pauseAsset_ = std::make_unique<PauseAsset>();
	pauseAsset_->Initilaize(blackTextureHandle, numberTextureHandleVector);

	//楽曲の再生
	audio_->Play(musicScoreData_.musicHandle, false);
	musicLength_= audio_->GetAudioLength(musicScoreData_.musicHandle);
}

void PlayMainScene::Update(){
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicScoreData_.musicHandle);

	//プレイ中
	if (isPlay_) {

		//上
		if (input_->IsTriggerKey(DIK_R)||input_->IsTriggerKey(DIK_I)) {
			upLaneCondition.isHit = true;
			upLaneCondition.touchTime = musicTime_;
		}
		else {
			upLaneCondition.isHit = false;
		}
		//下
		if (input_->IsTriggerKey(DIK_F) || input_->IsTriggerKey(DIK_J)) {
			downLaneCondition.isHit = true;
			downLaneCondition.touchTime = musicTime_;
		}
		else {
			downLaneCondition.isHit = false;
		}

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
	//ロングノーツのサンプルの更新
	longNoteSmaple_->Update();

	//判定線の更新
	judgementLineWorldTransform_.Update();
	judgementLineMaterial_.Update();
	
#ifdef _DEBUG
	ImGui::Begin("プレイシーン");
	ImGui::Checkbox("上ボタン", &upLaneCondition.isHit);
	ImGui::Checkbox("下ボタン", &downLaneCondition.isHit);
	ImGui::SliderFloat3("判定線のスケール", &judgementLineWorldTransform_.scale.x, 1.0f, 4.0f);
	ImGui::SliderFloat3("アンカーポイント", &judgementLineWorldTransform_.anchorPoint.x, -1.0f, 1.0f);

	ImGui::InputFloat("楽曲再生時間", &musicTime_);
	ImGui::InputFloat("楽曲の長さ", &musicLength_);
	ImGui::InputFloat("ポーズ時間", &restartTimer_);
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
	

	if(ImGui::TreeNode("上レーン")) {
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

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//通常ノーツの設定
	for(uint32_t i = 0u; i < NORMAL_NOTE_MAX_SIZE_; i++) {
		if (normalTapNoteArray_[i]->GetIsUsed()) {
			normalTapNoteArray_[i]->DrawObject3D(camera, baseLight);
		}
	}
	//ロングノーツのサンプルの描画
	longNoteSmaple_->DrawObject3D(camera, baseLight);

	//判定線の描画
	judgementLineModel_->Draw(judgementLineWorldTransform_, camera, judgementLineMaterial_, baseLight);

}

void PlayMainScene::DrawSprite(){
	//ポーズ中
	if (isPause_) {
		//アセットの描画
		pauseAsset_->Draw();
	}
	
}

void PlayMainScene::NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition){
	int32_t closestNoteIndex = -1;
	for (size_t i = 0u; i < noteInformations.size(); i++) {
		NoteInformation& note = noteInformations[i];

		//判定済みは処理せず次へ
		if (note.isJudged ) {
			continue;
		}
		//まだ動き始める時間になっていないので処理をしない
		if (note.startMoveTime > musicTime_) {
			break;
		}

		//タップ用
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
	Judge(noteInformations, laneCondition,closestNoteIndex);

	//通常ノーツの設定
	for (uint8_t i = 0u; i < NORMAL_NOTE_MAX_SIZE_; i++) {
		//使用時
		if (normalTapNoteArray_[i]->GetIsUsed()) {
			//楽曲時間を設定
			normalTapNoteArray_[i]->SetMusicTime(musicTime_);
			//更新
			normalTapNoteArray_[i]->Update();
		}
	}
}

void PlayMainScene::Judge(std::vector<NoteInformation>& noteInformation, LaneCondition& laneCondition, const int32_t& closestNoteIndex){
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
			if (absoluteJudgementTime >= 0.0f &&
				absoluteJudgementTime < NoteJudgement::Time::PERFECT) {

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
			else if (absoluteJudgementTime >= NoteJudgement::Time::PERFECT &&
				absoluteJudgementTime < NoteJudgement::Time::GREAT) {

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
			else if (absoluteJudgementTime >= NoteJudgement::Time::GREAT &&
				absoluteJudgementTime < NoteJudgement::Time::GOOD) {

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
			else if (absoluteJudgementTime >= NoteJudgement::Time::GOOD &&
				absoluteJudgementTime < NoteJudgement::Time::MISS) {

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

void PlayMainScene::Pause(){
	//ポーズ中
	if (isPause_) {
		
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			isRestart_ = true;
		}

		//再開処理
		if (isRestart_) {
			Restart();
		}

		//再開のカウントダウンの時間設定
		pauseAsset_->SetRestartTimer(restartTimer_);
		//アセットの更新
		pauseAsset_->Update();
	} 
	//これからポーズする
	else {
		//ESCAPEかウィンドウの移動中の時
		if (input_->IsTriggerKey(DIK_ESCAPE) ) {
			//ESCAPEでポーズ
			audio_->Stop(musicScoreData_.musicHandle);
			//音量も0にする
			audio_->ChangeVolume(musicScoreData_.musicHandle, 0.0f);
			//ポーズ時間の設定
			restartTimer_ = PAUSE_TIME_;
			//止めた時間を記録
			stopTime_ = musicTime_;
			//ポーズ中にする
			isPause_ = true;
			//まだ再生しない
			isResume_ = false;
		}

		//ウィンドウのタイトルバーに触れたときのコールバックを設定
		windowsSetup_->SetOnEnterSizeMoveCallback([this]() {
			//ESCAPEでポーズ
			audio_->Stop(musicScoreData_.musicHandle);
			//音量も0にする
			audio_->ChangeVolume(musicScoreData_.musicHandle, 0.0f);
			//ポーズ時間の設定
			restartTimer_ = PAUSE_TIME_;
			//止めた時間を記録
			stopTime_ = musicTime_;
			//ポーズ中にする
			isPause_ = true;
			//まだ再生しない
			isResume_ = false;
			}
		);
	}
}

void PlayMainScene::Restart(){
	//時間変化
	restartTimer_ -= DELTA_TIME_;

	//0になったら再開
	if (restartTimer_ <= 0.0f) {
		//少し前から再生する
		if (!isResume_) {
			audio_->Resume(musicScoreData_.musicHandle);
			isResume_ = true;
			//ポーズ解除
			isPause_ = false;
		}
	}
}