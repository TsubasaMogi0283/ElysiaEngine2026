#pragma once

/**
 * @file EndMainScene.h
 * @brief 終了シーンのクラス
 * @author 茂木翼
 */

#include <MainScene/BaseMainScene.h>

/// <summary>
/// 終了シーンのプレイシーン
/// </summary>
class EndMainScene :public BaseMainScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EndMainScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EndMainScene()override = default;

private:

	/// <summary>
	/// 待機
	/// </summary>
	void Wait();

	/// <summary>
	/// 結果の移動
	/// </summary>
	void MoveResult();

	/// <summary>
	/// 結果の表示
	/// </summary>
	void DisplayResult();

	/// <summary>
	/// トランジション
	/// </summary>
	void Transition();

private:
	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (EndMainScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (EndMainScene::* functionTable[])() = {
		&EndMainScene::Wait,
		&EndMainScene::MoveResult,
		&EndMainScene::DisplayResult,
		&EndMainScene::Transition,
	};

	/// <summary>
	/// 終了メインシーンの状態
	/// </summary>
	enum class EndMainSceneState {
		//待機
		Wait,
		//結果の移動
		MoveResult,
		//結果の表示
		DisplayResult,
		//トランジション
		Transition,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	EndMainSceneState currentState_ = EndMainSceneState::Wait;

	/// <summary>
	/// 結果のテクスチャの選択
	/// </summary>
	enum class ResultTextureSelection {
		//パーフェクト
		AllPerfect,
		//フルコンボ
		FullCombo,
		//クリア
		Complete,
		//失敗
		Failed,

		//量
		Amount,
	};


private:
	//待機時間
	const float_t WAITING_TIME_ = 2.0f;
	//準備時間
	const float_t READY_NEXT_TIME_ = 2.0f;

	//リザルトテクスチャの最大・最小サイズ
	const float_t RESULT_MAX_SIZE_ = 5.0f;
	const float_t RESULT_MIN_SIZE_ = 1.0f;

	//縮小時間
	const float_t RESULT_SCALE_DOWN_TIME_ = 0.5f;
	//表示時間
	const float_t MAX_DISPLAY_TIME_ = 2.0f;

private:
	//結果のスプライト
	std::unique_ptr<Elysia::Sprite> resultSprite_ = nullptr;
	//結果のテクスチャハンドル
	std::array<uint32_t, static_cast<size_t>(ResultTextureSelection::Amount)> resultTextureHandles_ = {};

	//待機時間
	float_t waitingTimeFormMain_ = 0.0f;

	//縮小の線形保管
	float_t resultScaleT_ = 0.0f;

	//表示時間
	float_t displayTime_ = 0.0f;

	//トランジションが終了したかどうか
	bool isEndTransition_=false;
	//次に進むまでの待機時間
	float_t readyNextTime_ = 0.0f;

};

