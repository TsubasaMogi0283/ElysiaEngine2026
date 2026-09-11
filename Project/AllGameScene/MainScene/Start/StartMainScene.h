#pragma once

/**
 * @file StartMainScene.h
 * @brief スタートメインシーンのクラス
 * @author 茂木翼
 */

#include <string>
#include <MainScene/BaseMainScene.h>

 /// <summary>
 /// メインシーンのスタートシーン
 /// </summary>
class StartMainScene :public BaseMainScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartMainScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight)override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartMainScene()override = default;


private:
	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum StartMainSceneState {
		//トランジション
		Transition,
		//UIの移動
		UIMove,
		//Ready
		Ready,
		//Go!!
		Go,
		//プレイシーンへ
		ToPlayScene,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	StartMainSceneState currentState_ = StartMainSceneState::Transition;


private:
	struct UITextTimeInformation {
		float_t startTime;
		float_t endTime;
	};

private:
	//UIの移動を待つ時間
	const float_t WAIT_FOR_UI_MOVE_TIME_ = 1.0f;
	//準備を待つ時間
	const float_t WAIT_FOR_READY_TIME_ = 2.0f;

	//Ready
	//テクスチャの量
	static const uint8_t READY_TEXTURE_AMOUNT_ = 5u;
	//テクスチャの名前
	const std::string READY_TEXTURE_NAME_ = "Ready";
	
	//Readyが動いている時間
	const float_t READY_SCALE_MOVE_TIME_ = 0.75f;
	//動く間隔
	const float_t READY_SCALE_MOVE_INTERVAL_ = 0.2f;
	const float_t READY_DISPLAY_TIME_ = 1.5f;

	//Go
	//テクスチャの量
	static const uint8_t GO_TEXTURE_AMOUNT_ = 3u;
	//テクスチャの名前
	const std::string GO_TEXTURE_NAME_ = "Go!";
	//サイズ
	const float_t GO_MAX_SCALE_ = 5.0f;
	const float_t GO_NORMAL_SCALE_ = 1.0f;
	const float_t GO_MIN_SCALE_ = 0.0f;

	//最初のスケールダウンの時間
	const float_t GO_FIRST_SCALE_DOWN_TIME_ = 0.75f;


private:
	bool isEndTransition = false;
	//Ready用のスプライト
	std::array<std::unique_ptr<Elysia::Sprite>, READY_TEXTURE_AMOUNT_> readySpriteArray_ = {};
	//
	std::array<UITextTimeInformation, READY_TEXTURE_AMOUNT_> readyScaleUpTime_ = {};
	std::array<UITextTimeInformation, READY_TEXTURE_AMOUNT_> readyScaleDownTime_ = {};
	
	//Readyに関する変数
	float_t allReadyStartTime_ = 0.0f;
	bool isNormalDisplayReady_ = false;
	bool isScaleDaownReady_ = false;
	bool isScaleDownReady_ = false;
	float_t readyDisplayTime_ = 0.0f;
	float_t scaleDownTime_ = 0.0f;
	bool isReadyWait_ = false;

	//Go用のスプライト
	std::array<std::unique_ptr<Elysia::Sprite>, GO_TEXTURE_AMOUNT_> goSpriteArray_ = {};

	//ReadyやGo!の下地
	std::unique_ptr<Elysia::Sprite> textBase_ = nullptr;
	//スケール
	Vector2<float_t> textBaseScale_ = { .x = 1.0f,.y = 1.0f };
	//スケールダウンの時間
	float_t goFirstScaleDownTime_ = 0.0f;
	float_t goSecondScaleDownTime_ = 0.0f;


	//UIの移動を待つ時間
	std::array<float_t, StartMainSceneState::Amount> waitingTimeArray_ = {};

	//線形補間(UI開始の動き)
	float_t startMoveT_ = 0.0f;

};

