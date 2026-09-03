#include "MainScene.h"

#include <cassert>
#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include <TextureManager.h>
#include <GameManager.h>
#include <MainScene/Start/StartMainScene.h>
#include <MainScene/Play/PlayMainScene.h>
#include <Note/NoteJudgement.h>

MainScene::MainScene() {
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//入力
	input_ = Elysia::Input::GetInstance();
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void MainScene::Initialize() {


	//楽曲譜面情報を取得
	musicInformation_ = gameManager_->GetMusicInformation();
#ifdef _DEBUG
	musicScoreData_ = gameManager_->GetScoreDataManager()->GetSampleMusicScoreData();
#endif // _DEBUG

	//ノーツの生成
	GenerateNotes();
	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("CollisionTest/CollisionTest.json");

	//カメラの初期化
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 5.0f,.z = -70.0f };

	//平行光源の初期化
	directionalLight_.Initialize();
	directionalLight_.direction = { 0.04f,-1.0f,0.75f };
	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();

	//数字のテクスチャハンドル
	for (uint8_t i = 0; i < NUMBER_TEXTURE_AMOUNT_; i++) {
		//各数字のテクスチャを読み込み
		std::string numberPath = "Resources/Sprite/Number/" + std::to_string(i) + ".png";
		numberTextureHandlesArray[i] = textureManager_->Load(numberPath);
	}

	//ゲージ
	uint32_t gaugeTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Gauge.png");
	gauge_.sprite = Elysia::Sprite::Create(gaugeTextureHandle);
	//画像サイズ
	uint64_t textureHeight = textureManager_->GetTextureHeight(gaugeTextureHandle);
	uint64_t textureWidth = textureManager_->GetTextureWidth(gaugeTextureHandle);
	//通常表示座標
	gaugeDisplayPosition_ = { .x = 640.0f - static_cast<float_t>(textureWidth / 2u),.y = 720.0f - static_cast<float_t>(textureHeight) };
	//初期座標
	initialGaugePosition_ = { gaugeDisplayPosition_.x, gaugeDisplayPosition_.y + textureHeight };


	//初期設定
	gauge_.sprite->SetPosition(initialGaugePosition_);
	gauge_.sprite->SetScale(gaugeScale);

	//数字のテクスチャの読み込み
	uint32_t numberTextureHandle[NUMBER_TEXTURE_AMOUNT_] = {};
	for (uint8_t i = 0u;i < NUMBER_TEXTURE_AMOUNT_;i++) {
		std::string fullPath = "Resources/Sprite/Number/" + std::to_string(i) + ".png";
		numberTextureHandle[i] = textureManager_->Load(fullPath);
	}

	//数字のテクスチャの横幅を取得
	numberTextureWidth = textureManager_->GetTextureWidth(numberTextureHandle[0]);
	numberTextureHeight = textureManager_->GetTextureHeight(numberTextureHandle[0]);

	//スコア
	for (uint8_t i = 0u; i < SCORE_DIGIT_; i++) {
		scoreArray_[i].sprite = Elysia::Sprite::Create(numberTextureHandle[i]);
		initialScorePositionY_ = -static_cast<float>(numberTextureHeight);
		scoreArray_[i].sprite->SetPosition({ .x = static_cast<float_t>(numberTextureWidth) * static_cast<float_t>(i)+100.0f,.y = initialScorePositionY_ });
	}
	scoreDisplayPositionY_ = 0.0f;

	//コンボ
	for (uint8_t i = 0u; i < COMBO_DIGIT_; i++) {
		//生成
		comboArray_[i].sprite = Elysia::Sprite::Create(numberTextureHandle[i]);
		comboArray_[i].position = {
			.x = (static_cast<float_t>(COMBO_DIGIT_-i) - 3.0f) * static_cast<float_t>(numberTextureWidth) + 640.0f,
			.y = 0.0f
		};
		comboArray_[i].sprite->SetPosition(comboArray_[i].position);
	}

	//メインシーンの中
	baseMainScene_ = std::make_unique<StartMainScene>();
#ifdef _DEBUG
	baseMainScene_ = std::make_unique<StartMainScene>();
#endif // _DEBUG
	baseMainScene_->SetMainScene(this);
	baseMainScene_->Initialize();

}

void MainScene::Update() {

#ifdef _DEBUG
	ImGui::Begin("メインシーン");
	ImGui::SliderFloat3("平行光源", &directionalLight_.direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat2("ゲージの座標", &gaugeDisplayPosition_.x, 0.0f, 720.0f);
	ImGui::SliderFloat2("ゲージのスケール", &gaugeScale.x, 0.0f, 1.0f);
	ImGui::SliderFloat4("色", &color_.x, 0.0f, 1.0f);
	ImGui::End();
	gauge_.sprite->SetScale(gaugeScale);
	gauge_.sprite->SetColor(color_);

	//リザルトへ
	if (input_->IsTriggerKey(DIK_N)) {
		gameManager_->ChangeScene("Result");
		return;
	}


#endif // _DEBUG

	//テクスチャ割り当て
	AssignToTexture();

	//更新
	levelDataManager_->Update(levelHandle_);
	baseMainScene_->Update();
	directionalLight_.Update();
	camera_.Update();
}

void MainScene::DrawObject3D() {
	//オブジェクトの描画
	//レベルエディタ  
	//levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);
	baseMainScene_->DrawObject3D(camera_, directionalLight_);

}

void MainScene::PreDrawPostEffect() {

	//ポストエフェクト描画前処理
	backTexture_->PreDraw();
}

void MainScene::DrawPostEffect() {
	//ポストエフェクト描画処理
	backTexture_->Draw();
}

void MainScene::DrawSprite() {
	
	//ゲージ
	gauge_.sprite->Draw();

	//スコア
	for (uint8_t i = 0u;i < SCORE_DIGIT_;i++) {
		scoreArray_[i].sprite->Draw(scoreArray_[0].textureHandle);
	}

	//コンボ
	for (uint8_t i = 0u;i < COMBO_DIGIT_;i++) {
		if (totalCombo_ >= 10u) {

			comboArray_[i].sprite->Draw(comboArray_[i].textureHandle);
		}
	}


	//スプライトの描画
	baseMainScene_->DrawSprite();


}

void MainScene::GenerateNotes() {
	//合計の時間
	float_t totalTime = 0.0f;
	//開始時間の設定
	float_t startTime = START_OFFSET_TIME_ * hiSpeed_;
	//ノーツの配置
	for (const NoteBarInformation& bar : musicScoreData_.newNotesData) {
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
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::NormalTap,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::NormalTap) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::NormalTap,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region ハイパスロングノーツ
			//上
			if (note.upNote == NoteType::HiPassLongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::HiPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::HiPassLongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::HiPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region ローパスロングノーツ
			//上
			if (note.upNote == NoteType::LowPassLongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::LowPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::LowPassLongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::LowPassLongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
			}
#pragma endregion

#pragma region トランスゲート8分ロングノーツ

			//上
			if (note.upNote == NoteType::TranceGate8LongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::TranceGate8LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::TranceGate8LongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::TranceGate8LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
			}


#pragma endregion


#pragma region トランスゲート16分ロングノーツ

			//上
			if (note.upNote == NoteType::TranceGate16LongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Up),
					.length = length,
					.type = NoteType::TranceGate16LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);

			}
			//下
			if (note.downNote == NoteType::TranceGate16LongStart) {
				//ノーツの数を増やす
				musicScoreData_.totalNote_++;
				//ノーツ情報の設定
				NoteInformation noteInformation = {
					.place = static_cast<uint8_t>(NoteLane::Place::Down),
					.length = length,
					.type = NoteType::TranceGate16LongStart,
					.startMoveTime = totalTime + i * noteInterval - startTime,
					.arriveLineTime = totalTime + i * noteInterval,
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.downInformation.push_back(noteInformation);
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
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
				};
				//挿入
				musicScoreData_.upInformation.push_back(noteInformation);
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
					.moveRatio = 0.0f,
					.judgement = NoteJudgement::Selection::None,
					.isJudged = false,
					.isAssigned = false
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

void MainScene::AssignToTexture() {

#pragma region スコア
	//各桁に数字を割り当てる
	totalScore_ = 1234567u;
	uint32_t score = totalScore_;
	uint32_t scoreDigit = 1000000u;
	for (uint8_t i = SCORE_DIGIT_ - 1u; i > 0u; i--) {
		scoreArray_[i].value = static_cast<uint8_t>(score / scoreDigit);
		score %= scoreDigit;
		//10のくらいの時以外だけ
		if (i != TEN_DIGIT_) {
			scoreDigit /= 10;
		}
	}
	//1の位は個別でやる
	scoreArray_[ONE_DIGIT_].value = static_cast<uint8_t>(score) % static_cast<uint8_t>(scoreDigit);

	//テクスチャハンドルに割り当てる
	scoreArray_[ONE_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[ONE_DIGIT_].value];
	scoreArray_[TEN_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[TEN_DIGIT_].value];
	scoreArray_[ONE_HUNDRED_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[ONE_HUNDRED_DIGIT_].value];
	scoreArray_[ONE_THOUSAND_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[ONE_THOUSAND_DIGIT_].value];
	scoreArray_[TEN_THOUSAND_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[TEN_THOUSAND_DIGIT_].value];
	scoreArray_[ONE_HUNDRED_THOUSAND_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[ONE_HUNDRED_THOUSAND_DIGIT_].value];
	scoreArray_[ONE_MILLION_DIGIT_].textureHandle = numberTextureHandlesArray[scoreArray_[ONE_MILLION_DIGIT_].value];

#pragma endregion

#pragma region コンボ

#ifdef _DEBUG
	totalCombo_ = 123u;
#endif // _DEBUG
	uint16_t combo = totalCombo_;

	//各桁の数字を割り当てる
	for (uint8_t i = 0;i < COMBO_DIGIT_;i++) {
		comboArray_[i].value = static_cast<uint16_t>(combo % 10u);
		//最後の桁以外は10で割る
		if (i != COMBO_DIGIT_-1u) {
			combo /= 10u;
		}
		//テクスチャハンドルに割り当てる
		comboArray_[i].textureHandle = numberTextureHandlesArray[comboArray_[i].value];
	}

	
	
#ifdef _DEBUG
	ImGui::Begin("コンボ");
	int one = comboArray_[ONE_DIGIT_].value;
	int ten = comboArray_[TEN_DIGIT_].value;
	int hundred = comboArray_[ONE_HUNDRED_DIGIT_].value;
	int thousand = comboArray_[ONE_THOUSAND_DIGIT_].value;

	ImGui::InputInt("千", &thousand);
	ImGui::InputInt("百", &hundred);
	ImGui::InputInt("十", &ten);
	ImGui::InputInt("一", &one);
	ImGui::InputFloat2("千",&comboArray_[ONE_THOUSAND_DIGIT_].position.x);
	ImGui::InputFloat2("百", &comboArray_[ONE_HUNDRED_DIGIT_].position.x);
	ImGui::InputFloat2("十", &comboArray_[TEN_DIGIT_].position.x);
	ImGui::InputFloat2("一", &comboArray_[ONE_DIGIT_].position.x);
	ImGui::End();
#endif // _DEBUG



#pragma endregion
}