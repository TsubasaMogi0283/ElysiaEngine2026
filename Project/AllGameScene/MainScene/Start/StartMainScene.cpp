#include "StartMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Easing.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <TextureManager.h>
#include <WindowsSetup.h>
#include <MainScene/Play/PlayMainScene.h>

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
	const Vector2<int32_t> WINDOW_SIZE = {
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
			.startTime= READY_SCALE_MOVE_INTERVAL_ * i,
			.endTime = READY_SCALE_MOVE_INTERVAL_ * i + READY_SCALE_MOVE_TIME_ 
		};

		//文字の動く時間を設定
		readyScaleDownTime_[i] = {
			.startTime = 0.0f,
			.endTime = READY_SCALE_MOVE_INTERVAL_
		};

		//テクスチャサイズ
		Vector2<int32_t> readyTextureSize = {
			.x= static_cast<int32_t>(textureManager_->GetTextureWidth(readyTextureHandle)),
			.y= static_cast<int32_t>(textureManager_->GetTextureHeight(readyTextureHandle))
		};

		//アンカーポイント
		readySpriteArray_[i]->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });

		//座標の設定
		readySpriteArray_[i]->SetPosition({
			.x = WINDOW_SIZE.x / 2 - (2-i) * (readyTextureSize.x),
			.y = WINDOW_SIZE.y / 2
			}
		);
		//最初は非表示にする
		readySpriteArray_[i]->SetInvisible(true);
	}

	//Goのスプライトの生成
	for (uint8_t i = 0; i < GO_TEXTURE_AMOUNT_; i++) {
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
		goSpriteArray_[i]->SetPosition({ .x = WINDOW_SIZE.x/2-(i-1)* goTextureSize.x,.y = WINDOW_SIZE.y/2 });

	}

	//下地のスプライトの生成
	textBase_ = Elysia::Sprite::Create();
	//最初は非表示
	textBase_->SetInvisible(true);
	//アンカーポイントの設定
	textBase_->SetAnchorPoint({ .x	= 0.0f,.y = 0.5f });

	//スケール
	textBaseScale_ = { .x = 1.0f,.y = static_cast<float_t>(goTextureSize.y) / static_cast<float_t>(WINDOW_SIZE.y) };
	textBase_->SetScale(textBaseScale_);
	//座標
	textBase_->SetPosition({ .x = 0,.y = WINDOW_SIZE.y / 2 });
}

void StartMainScene::Update() {

	float_t gaugePositionY = 0;
	float_t scorePositionY = 0;
	float_t easedT = 0.0f;
	float_t textBaseEaseT = 0.0f;
	float_t textBaseT = 0.0f;
	//状態遷移
	//ローカル変数の宣言がswitchの中でできないの腹立つので関数ポインタでやっていきたい。
	switch (currentState_) {
	case StartMainSceneState::Transition:

		//トランジションから始まる
		if (mainScene_->GetGameManager()->GetTransition()->SetOpenTransition()) {
			isEndTransition = true;
		}

		if (isEndTransition) {
			waitingTimeArray_[Transition] += DELTA_TIME_;
			if (waitingTimeArray_[Transition] >= WAIT_FOR_UI_MOVE_TIME_) {
				//トランジションが終わったらUIの移動へ
				currentState_ = StartMainSceneState::UIMove;
			}
		}
		break;

	case StartMainSceneState::UIMove:

		//線形補間の時間を加算
		startMoveT_ += DELTA_TIME_ * 2.0f;
		startMoveT_ = std::clamp(startMoveT_, 0.0f, 1.0f);
		//イージング
		//種類はそろえた方が統一感が出るのでEaseInOutQuadに統一する
		easedT = Easing::EaseInOutQuad(startMoveT_);

		//ゲージ
		gaugePositionY = SingleCalculation::Lerp(static_cast<float_t>(mainScene_->GetInitialGaugePosition().y), static_cast<float_t>(mainScene_->GetGaugeDisplayPosition().y), easedT);
		mainScene_->SetGaugePosition({ mainScene_->GetGaugeDisplayPosition().x, static_cast<int32_t>(gaugePositionY)});

		//スコア
		scorePositionY = SingleCalculation::Lerp(static_cast<float_t>(mainScene_->GetInitialScorePositionY()), static_cast<float_t>(mainScene_->GetScoreDisplayPositionY()), easedT);
		mainScene_->SetScorePositionsY(static_cast<int32_t>(scorePositionY));

		//指定した時間を超えたらReadyへ
		if (easedT >= 1.0f) {
			waitingTimeArray_[UIMove] += DELTA_TIME_;
			if (waitingTimeArray_[UIMove] >= WAIT_FOR_READY_TIME_) {
				currentState_ = StartMainSceneState::Ready;
			}
		}
		break;

	case StartMainSceneState::Ready:
		//表示
		textBase_->SetInvisible(false);

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

		//スケールの設定
		textBaseT = SingleCalculation::InverseLerp(0.0f, 1.0f, allReadyStartTime_);
		textBaseT = std::clamp(textBaseT, 0.0f, 1.0f);
		textBaseEaseT=Easing::EaseInOutQuart(textBaseT);
		
		textBaseScale_.y = static_cast<float_t>(goTextureSize.y) / static_cast<float_t>(720) *textBaseEaseT;
		textBase_->SetScale(textBaseScale_);

		if (isNormalDisplayReady_) {
			readyDisplayTime_ += DELTA_TIME_;

			if (readyDisplayTime_ >= READY_DISPLAY_TIME_) {
				isScaleDownReady_ = true;
			}
		}

		if (isScaleDownReady_) {
			scaleDownTime_ += DELTA_TIME_;

			//スケールダウンの処理
			for (uint8_t i = 0u;i < READY_TEXTURE_AMOUNT_;i++) {
				float_t t = SingleCalculation::InverseLerp(readyScaleDownTime_[i].startTime, readyScaleDownTime_[i].endTime, scaleDownTime_);
				t = std::clamp(t, 0.0f, 1.0f);
				float_t easeT = Easing::EaseInQuart(t);
				readySpriteArray_[i]->SetScale({ .x = 1.0f- easeT,.y = 1.0f - easeT });

				//Goへ
				if (t >= 1.0f) {
					isReadyWait_ = true;
				}
			}

			waitingTimeArray_[StartMainSceneState::Ready] += DELTA_TIME_;
			if (waitingTimeArray_[StartMainSceneState::Ready] >= 2.0f) {
				currentState_ = StartMainSceneState::Go;
			}	
		}
		break;

	case StartMainSceneState::Go:

		//スケールダウンの時間
		goFirstScaleDownTime_ += DELTA_TIME_;

		//スケールダウンの処理
		for (uint8_t i = 0u;i < GO_TEXTURE_AMOUNT_;i++) {
			//線形補間とイージングで滑らかにスケールダウン
			float_t t = SingleCalculation::InverseLerp(0.0f, GO_FIRST_SCALE_DOWN_TIME_, goFirstScaleDownTime_);
			t = std::clamp(t, 0.0f, 1.0f);
			float_t easeT = Easing::EaseOutQuart(t);
			float_t textScale = SingleCalculation::Lerp(GO_MAX_SCALE_, GO_NORMAL_SCALE_, easeT);
			
			goSpriteArray_[i]->SetScale({ .x = 1.0f,.y = textScale});
		}


		//Go!!の表示
		break;

	case StartMainSceneState::ToPlayScene:
		//プレイシーンへ

		break;
	}

#ifdef _DEBUG
	ImGui::Begin("メインシーン(開始)");
	ImGui::InputFloat("時間", &allReadyStartTime_);
	ImGui::InputFloat("開始線形補間の値", &startMoveT_);
	ImGui::InputFloat("sss", &scorePositionY);
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

	switch (currentState_) {
	case StartMainSceneState::Ready:
		//Readyの描画
		for (uint8_t i = 0; i < READY_TEXTURE_AMOUNT_; i++) {
			readySpriteArray_[i]->Draw();
		}

		break;
	case StartMainSceneState::Go:
		//Goのスプライトの描画
		for (uint8_t i = 0; i < GO_TEXTURE_AMOUNT_; i++) {
			goSpriteArray_[i]->Draw();
		}

		break;
	}
}
