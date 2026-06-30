#include "MainScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include <GameManager.h>
#include <MainScene/Start/StartMainScene.h>
#include <Note/NoteJudgement.h>

MainScene::MainScene(){
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//入力
	input_ = Elysia::Input::GetInstance();
}

void MainScene::Initialize(){


	//楽曲譜面情報を取得
	musicInformation_ = gameManager_->GetMusicInformation();
	musicScoreData_ = gameManager_->GetScoreDataManager()->GetSampleMusicScoreData();
	
	//この2つスレッドでできるんじゃない？
	//ノーツの生成
	GenerateNotes();
	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("CollisionTest/CollisionTest.json");

	//カメラの初期化
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float_t> / 6.0f;
	camera_.translate = { .x = 0.0f,.y = 21.0f,.z = -40.0f };

	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();
	
	//メインシーンの中
	baseMainScene_ = std::make_unique<StartMainScene>();
	baseMainScene_->SetMainScene(this);
	baseMainScene_->Initialize();

}

void MainScene::Update(){

	

#ifdef _DEBUG
	ImGui::Begin("メインシーン");
	ImGui::SliderFloat3("平行光源", &directionalLight_.direction.x,-1.0f,1.0f);
	ImGui::End();
	//リザルトへ
	if (input_->IsTriggerKey(DIK_N)) {
		gameManager_->ChangeScene("Result");
		return;
	}
	
#endif // _DEBUG


	//更新
	levelDataManager_->Update(levelHandle_);
	baseMainScene_->Update();
	directionalLight_.Update();
	camera_.Update();
}

void MainScene::DrawObject3D(){
	//オブジェクトの描画
	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);
	baseMainScene_->DrawObject3D(camera_,directionalLight_);

}

void MainScene::PreDrawPostEffect(){
	
	//ポストエフェクト描画前処理
	baseMainScene_->PreDrawPostEffect();
	backTexture_->PreDraw();
}

void MainScene::DrawPostEffect(){
	//ポストエフェクト描画処理
	baseMainScene_->DrawPostEffect();
	backTexture_->Draw();
}

void MainScene::DrawSprite(){
	//スプライトの描画
	baseMainScene_->DrawSprite();
}

void MainScene::GenerateNotes(){
	//合計の時間
	float_t totalTime = 0.0f;
	//開始時間の設定
	float_t startTime = START_OFFSET_TIME_ - hiSpeed_;

	//初期座標
	Vector3 initialPosition = { .x = INITIAL_POSITION_X_ ,.y = 0.0f,.z = LANE_POSITION_Z_ };
	//判定座標
	Vector3 judgementPosition = { .x = JUDGEMENT_POSITION_X_ ,.y = 0.0f,.z = LANE_POSITION_Z_ };
	//ノーツの配置
	for (const NoteBarInformation& bar : musicScoreData_.newNotesData) {
		//1拍の秒数
		float_t beatDuration = 60.0f / bar.bpm;
		//ノーツ間隔(1小節4拍)
		float_t noteInterval = (beatDuration * 4.0f) / bar.notesLane.size();
		//長さ
		uint8_t noteLength = static_cast<uint8_t>(bar.notesLane.size());

		for (size_t i = 0u; i < bar.notesLane.size(); i++) {
			const auto& note = bar.notesLane[i];
			
#pragma region 通常タッチ
			//上
			if (note.upNote == NoteType::NormalTap) {
				//初期Y座標を設定
				initialPosition.y = LANE_POSITION_Y_[NoteLane::Place::Up];
				//判定Y座標を設定
				judgementPosition.y = LANE_POSITION_Y_[NoteLane::Place::Up];

				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = NoteLane::Place::Up,
					.noteLength = noteLength,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveTime = totalTime + i * noteInterval,
					.initialPosition = initialPosition,
					.currentPosition = initialPosition,
					.moveRatio = 0.0f,
					.isDisplay = true,
					.isProcessEnd = false,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.note = nullptr
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::NormalTap) {
				//初期Y座標を設定
				initialPosition.y = LANE_POSITION_Y_[NoteLane::Place::Down];
				//判定Y座標を設定
				judgementPosition.y = LANE_POSITION_Y_[NoteLane::Place::Down];

				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = NoteLane::Place::Up,
					.noteLength = noteLength,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveTime = totalTime + i * noteInterval,
					.initialPosition = initialPosition,
					.currentPosition = initialPosition,
					.moveRatio = 0.0f,
					.isDisplay = true,
					.isProcessEnd = false,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.note = nullptr
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);

			}
#pragma endregion

#pragma region ロング終点
			//上
			if (note.upNote == NoteType::LongEnd) {
				//初期Y座標を設定
				initialPosition.y = LANE_POSITION_Y_[NoteLane::Place::Up];
				//判定Y座標を設定
				judgementPosition.y = LANE_POSITION_Y_[NoteLane::Place::Up];

				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = NoteLane::Place::Up,
					.noteLength = noteLength,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveTime = totalTime + i * noteInterval,
					.initialPosition = initialPosition,
					.currentPosition = initialPosition,
					.moveRatio = 0.0f,
					.isDisplay = true,
					.isProcessEnd = false,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.note = nullptr
				};

				musicScoreData_.upInformation.push_back(noteInformation);

			}
			
			//下
			if (note.downNote == NoteType::LongEnd) {
				//初期Y座標を設定
				initialPosition.y = LANE_POSITION_Y_[NoteLane::Place::Down];
				//判定Y座標を設定
				judgementPosition.y = LANE_POSITION_Y_[NoteLane::Place::Down];

				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報を設定
				NoteInformation noteInformation = {
					.place = NoteLane::Place::Up,
					.noteLength = noteLength,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveTime = totalTime + i * noteInterval,
					.initialPosition = initialPosition,
					.currentPosition = initialPosition,
					.moveRatio = 0.0f,
					.isDisplay = true,
					.isProcessEnd = false,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.note = nullptr
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

		}
		// 小節分進める
		totalTime += 4.0f * beatDuration;
	}
}

