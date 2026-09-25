#include "StartMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Easing.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <TextureManager.h>
#include <WindowsSetup.h>
#include <MainScene/Play/PlayMainScene.h>
#include <Color.h>

StartMainScene::StartMainScene() {
	//インスタンスの取得
	input_ = Elysia::Input::GetInstance();
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void StartMainScene::Initialize() {
	//メインシーンの空チェック
	assert(mainScene_);

	//ウィンドウのサイズ
	windowSize_ = {
		.x = static_cast<int32_t>(Elysia::WindowsSetup::GetInstance()->GetClientSize().x),
		.y = static_cast<int32_t>(Elysia::WindowsSetup::GetInstance()->GetClientSize().y)
	};

	//Ready
	for (uint8_t i = 0; i < READY_TEXTURE_AMOUNT_; i++) {
		//スプライトの生成
		std::string fullPath = "Resources/Sprite/ReadyGo/Ready/" + std::string(1, READY_TEXTURE_NAME_[i]);
		uint32_t readyTextureHandle = textureManager_->Load(fullPath + ".png");
		readySpriteArray_[i] = Elysia::Sprite::Create(readyTextureHandle);

		//文字の動く時間を設定
		readyScaleUpTime_[i] = {
			.startTime = READY_SCALE_MOVE_INTERVAL_ * i,
			.endTime = READY_SCALE_MOVE_INTERVAL_ * i + READY_SCALE_MOVE_TIME_
		};

		//文字の動く時間を設定
		readyScaleDownTime_[i] = {
			.startTime = 0.0f,
			.endTime = READY_SCALE_MOVE_INTERVAL_
		};

		//テクスチャサイズ
		Vector2<int32_t> readyTextureSize = {
			.x = static_cast<int32_t>(textureManager_->GetTextureWidth(readyTextureHandle)),
			.y = static_cast<int32_t>(textureManager_->GetTextureHeight(readyTextureHandle))
		};

		//アンカーポイント
		readySpriteArray_[i]->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });

		//座標の設定
		readySpriteArray_[i]->SetPosition({
			.x = windowSize_.x / 2 - (2 - i) * (readyTextureSize.x),
			.y = windowSize_.y / 2
			}
		);
		//最初は非表示にする
		readySpriteArray_[i]->SetInvisible(true);
	}

	//Goのスプライトの生成
	for (uint8_t i = 0; i < GO_TEXTURE_AMOUNT_; i++) {
		//パス
		std::string fullPath = "Resources/Sprite/ReadyGo/Go/" + std::string(1, GO_TEXTURE_NAME_[i]);
		uint32_t goTextureHandle = textureManager_->Load(fullPath + ".png");

		//サイズ
		goTextureSize = {
			.x = static_cast<int32_t>(textureManager_->GetTextureWidth(goTextureHandle)) ,
			.y = static_cast<int32_t>(textureManager_->GetTextureHeight(goTextureHandle))
		};

		//生成
		goSpriteArray_[i] = Elysia::Sprite::Create(goTextureHandle);
		//アンカーポイント
		goSpriteArray_[i]->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
		//座標
		goSpriteArray_[i]->SetPosition({ .x = windowSize_.x / 2 - (i - 1) * goTextureSize.x,.y = windowSize_.y / 2 });
		//非表示
		goSpriteArray_[i]->SetInvisible(true);

	}

	//下地のスプライトの生成
	textBase_ = Elysia::Sprite::Create();
	//最初は非表示
	textBase_->SetInvisible(true);
	//アンカーポイントの設定
	textBase_->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });

	//スケール
	textBaseScale_ = { .x = 1.0f,.y = static_cast<float_t>(goTextureSize.y) / static_cast<float_t>(windowSize_.y) };
	textBase_->SetScale(textBaseScale_);
	//座標
	textBase_->SetPosition({ .x = windowSize_.x / 2,.y = windowSize_.y / 2 });
	//サイズ
	int32_t baseSizeY = static_cast<int32_t>(static_cast<float_t>(windowSize_.y) * textBaseScale_.y);

	//BPMの取得
	bpm_ = static_cast<int32_t>(mainScene_->GetScoreData().musicInformation.bpm);
	//レベルの取得
	level_ = mainScene_->GetScoreData().musicInformation.level;

	//線
	//レベルによって色を変える
	Vector4 levelColor = {};
	if (level_ == "Easy") {
		levelColor = Color::Conevert(Color::EASY);
	} else if (level_ == "Normal") {
		levelColor = Color::Conevert(Color::NORMAL);
	} else if (level_ == "Hard") {
		levelColor = Color::Conevert(Color::HARD);
	} else if (level_ == "Special") {
		levelColor = Color::Conevert(Color::SPECIAL);
	}

	//スプライトの設定
	for (uint8_t i = 0u;i < LINE_AMOUNUT_;i++) {
		//生成
		baseLineSpriteArray_[i] = Elysia::Sprite::Create();
		//アンカーポイント
		baseLineSpriteArray_[i]->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
		//座標
		baseLineSpriteArray_[i]->SetPosition({ .x = windowSize_.x / 2,.y = windowSize_.y / 2 - baseSizeY / 2 + i * baseSizeY });
		//スケール
		baseLineSpriteArray_[i]->SetScale({ .x = 0.0f,.y = LINE_SCALE_ });
		//色
		baseLineSpriteArray_[i]->SetColor(levelColor);
	}
}

void StartMainScene::Update() {

	//各状態を実行
	(this->*functionTable[static_cast<size_t>(currentState_)])();

	//全ての状態の処理が終わったらいざ遊ぶシーンへ！
	if (isProcessEnd_) {
		mainScene_->ChangeMainScene(std::make_unique<PlayMainScene>());
		return;
	}

#ifdef _DEBUG
	ImGui::Begin("メインシーン(開始)");
	ImGui::InputFloat("時間", &allReadyStartTime_);
	ImGui::InputFloat("開始線形補間の値", &startMoveTime_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
		mainScene_->ChangeMainScene(std::make_unique<PlayMainScene>());
		return;
	}

#endif // _DEBUG
}

void StartMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight) {
	camera;
	baseLight;
}

void StartMainScene::DrawSprite() {

	//テキストの下地
	textBase_->Draw();

	//線
	for (uint8_t i = 0u;i < LINE_AMOUNUT_;i++) {
		//生成
		baseLineSpriteArray_[i]->Draw();
	}

	//Readyの描画
	for (uint8_t i = 0; i < READY_TEXTURE_AMOUNT_; i++) {
		readySpriteArray_[i]->Draw();
	}

	//Goのスプライトの描画
	for (uint8_t i = 0; i < GO_TEXTURE_AMOUNT_; i++) {
		goSpriteArray_[i]->Draw();
	}

}

void StartMainScene::Transition() {
	//トランジションから始まる
	if (mainScene_->GetGameManager()->GetTransition()->SetOpenTransition()) {
		isEndTransition = true;
	}

	if (isEndTransition) {
		waitingTimeArray_[static_cast<size_t>(StartMainSceneState::Transition)] += DELTA_TIME_;
		if (waitingTimeArray_[static_cast<size_t>(StartMainSceneState::Transition)] >= WAIT_FOR_UI_MOVE_TIME_) {
			//トランジションが終わったらUIの移動へ 
			currentState_ = StartMainSceneState::UIMoveScaleUp;
		}
	}
}

void StartMainScene::UIScaleUp(){
	//線形補間の時間を加算
	startMoveTime_ += DELTA_TIME_;
	float_t startMoveT = SingleCalculation::InverseLerp(0.0f, UI_MOVE_TIME_, startMoveTime_);
	startMoveT = std::clamp(startMoveT, 0.0f, 1.0f);
	//イージング
	//種類はそろえた方が統一感が出るのでEaseInOutQuadに統一する
	float_t easedT = Easing::EaseInOutQuad(startMoveT);

	//ゲージ
	float_t gaugePositionY = SingleCalculation::Lerp(
		static_cast<float_t>(mainScene_->GetInitialGaugePosition().y),
		static_cast<float_t>(mainScene_->GetGaugeDisplayPosition().y),
		easedT);
	mainScene_->SetGaugePosition({ mainScene_->GetGaugeDisplayPosition().x, static_cast<int32_t>(gaugePositionY) });

	//スコア
	float_t scorePositionY = SingleCalculation::Lerp(static_cast<float_t>(mainScene_->GetInitialScorePositionY()), static_cast<float_t>(mainScene_->GetScoreDisplayPositionY()), easedT);
	mainScene_->SetScorePositionsY(static_cast<int32_t>(scorePositionY));

	//下地
	//表示
	textBase_->SetInvisible(false);
	//スケールの設定
	float_t textBaseT = SingleCalculation::InverseLerp(0.0f, 2.0f, startMoveTime_);
	textBaseT = std::clamp(textBaseT, 0.0f, 1.0f);
	float_t textBaseEaseT = Easing::EaseInOutQuart(textBaseT);
	textBaseScale_.x = textBaseEaseT;
	textBase_->SetScale(textBaseScale_);
	//線
	for (uint8_t i = 0u;i < LINE_AMOUNUT_;i++) {
		//生成
		baseLineSpriteArray_[i]->SetScale({ .x = textBaseEaseT,.y = LINE_SCALE_ });
	}

	//指定した時間を超えたらReadyへ
	if (easedT >= 1.0f) {
		waitingTimeArray_[static_cast<size_t>(StartMainSceneState::UIMoveScaleUp)] += DELTA_TIME_;
		if (waitingTimeArray_[static_cast<size_t>(StartMainSceneState::UIMoveScaleUp)] >= WAIT_FOR_READY_TIME_) {
			currentState_ = StartMainSceneState::Ready;
		}
	}
}

void StartMainScene::Ready(){

	//Readyの表示
	allReadyStartTime_ += DELTA_TIME_;
	for (uint8_t i = 0u;i < READY_TEXTURE_AMOUNT_;i++) {
		readySpriteArray_[i]->SetInvisible(false);

		float_t t = SingleCalculation::InverseLerp(readyScaleUpTime_[i].startTime, readyScaleUpTime_[i].endTime, allReadyStartTime_);
		t = std::clamp(t, 0.0f, 1.0f);
		float_t easeT = Easing::EaseOutBack(t);
		readySpriteArray_[i]->SetScale({ .x = 1.0f,.y = easeT });

		if (t >= 1.0f) {
			isNormalDisplayReady_ = true;
		}
	}

	//通常表示
	if (isNormalDisplayReady_) {
		readyDisplayTime_ += DELTA_TIME_;

		if (readyDisplayTime_ >= READY_DISPLAY_TIME_) {
			isScaleDownReady_ = true;
		}
	}

	//Ready
	if (isScaleDownReady_) {
		scaleDownTime_ += DELTA_TIME_;

		//スケールダウンの処理
		for (uint8_t i = 0u;i < READY_TEXTURE_AMOUNT_;i++) {
			float_t t = SingleCalculation::InverseLerp(readyScaleDownTime_[i].startTime, readyScaleDownTime_[i].endTime, scaleDownTime_);
			t = std::clamp(t, 0.0f, 1.0f);
			float_t easeT = Easing::EaseInQuart(t);
			readySpriteArray_[i]->SetScale({ .x = 1.0f,.y = 1.0f - easeT });

			//Goへ
			if (t >= 1.0f) {
				isReadyWait_ = true;
			}
		}

		waitingTimeArray_[static_cast<size_t>(StartMainSceneState::Ready)] += DELTA_TIME_;
		if (waitingTimeArray_[static_cast<size_t>(StartMainSceneState::Ready)] >= 2.0f) {
			isScaleDown_ = true;
			currentState_ = StartMainSceneState::Go;
		}
	}

}

void StartMainScene::Go(){

	//スケールダウンの時間
	if (isScaleDown_) {
		
		goFirstScaleDownTime_ += DELTA_TIME_;

		//線形補間とイージングで滑らかにスケールダウン
		float_t goScaleDownT = SingleCalculation::InverseLerp(0.0f, GO_SCALE_DOWN_TIME_, goFirstScaleDownTime_);
		goScaleDownT = std::clamp(goScaleDownT, 0.0f, 1.0f);
		float_t goScaleDownEaseT = Easing::EaseOutQuart(goScaleDownT);
		float_t goTextScale = SingleCalculation::Lerp(GO_MAX_SCALE_, GO_NORMAL_SCALE_, goScaleDownEaseT);

		if (goScaleDownT >= 1.0f) {
			goDisplayTime_ += DELTA_TIME_;
		}

		//表示時間が過ぎたらTiPLay
		if (goDisplayTime_ > GO_DISPLAY_TIME_) {
			isScaleDown_ = false;
			isDeleteScaleDown_ = true;
		}
		//スケールダウンの処理
		for (uint8_t i = 0u;i < GO_TEXTURE_AMOUNT_;i++) {
			goSpriteArray_[i]->SetInvisible(false);
			goSpriteArray_[i]->SetScale({ .x = goTextScale,.y = goTextScale });
		}

	}

	//消していく
	if (isDeleteScaleDown_) {
		goDeleteTime_ += DELTA_TIME_;
		//線形補間とイージングで滑らかにスケールダウン
		float_t goDeleteT = SingleCalculation::InverseLerp(0.0f, GO_SCALE_DOWN_TIME_, goDeleteTime_);
		goDeleteT = std::clamp(goDeleteT, 0.0f, 1.0f);
		float_t goDeleteEaseT = Easing::EaseOutQuart(goDeleteT);
		float_t goTextScale = SingleCalculation::Lerp(GO_NORMAL_SCALE_, 0.0f, goDeleteEaseT);

		//スケールダウンの処理
		for (uint8_t i = 0u;i < GO_TEXTURE_AMOUNT_;i++) {
			goSpriteArray_[i]->SetScale({ .x = 1.0f,.y = goTextScale });
		}

		//消えたらBPMチェックへ
		if (goDeleteT >= 1.0f) {
			currentState_ = StartMainSceneState::UIScaleDown;
		}
	}
}

void StartMainScene::UIScaleDown(){
	//時間
	baseScaleTime_ += DELTA_TIME_;
	//スケールの設定
	float_t textBaseT = SingleCalculation::InverseLerp(0.0f, 1.0f, baseScaleTime_);
	textBaseT = std::clamp(textBaseT, 0.0f, 1.0f);
	float_t textBaseEaseT = Easing::EaseInOutQuart(textBaseT);
	textBaseScale_.x = 1.0f - textBaseEaseT;
	textBase_->SetScale(textBaseScale_);
	//線
	for (uint8_t i = 0u;i < LINE_AMOUNUT_;i++) {
		//生成
		baseLineSpriteArray_[i]->SetScale({ .x = 1.0f - textBaseEaseT,.y = LINE_SCALE_ });
	}

	//プレイシーンへ
	if (textBaseT >= 1.0f) {
		currentState_ = StartMainSceneState::ChechTempo;
		isProcessEnd_ = true;
	}
}

