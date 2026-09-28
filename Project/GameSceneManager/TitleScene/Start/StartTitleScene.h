#pragma once

/**
 * @file StartTitleScene.h
 * @brief タイトルシーンの開始クラス
 * @author 茂木翼
 */


#include <TitleScene/BaseTitleScene.h>

/// <summary>
/// タイトルシーンの開始
/// </summary>
class StartTitleScene :public BaseTitleScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartTitleScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="baseLight">ライト</param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight)override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartTitleScene() = default;

private:
private:
	/// <summary>
	/// トランジション
	/// </summary>
	void Transition();

	/// <summary>
	/// UIの拡大
	/// </summary>
	void UIScaleUp();

	/// <summary>
	/// 準備
	/// </summary>
	void Ready();

	/// <summary>
	/// Go
	/// </summary>
	void Go();

	/// <summary>
	/// UIの縮小
	/// </summary>
	void UIScaleDown();

private:
	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (StartTitleScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (StartTitleScene::* functionTable[])() = {
		&StartTitleScene::Transition,
		& StartTitleScene::UIScaleUp,
		& StartTitleScene::Ready,
		& StartTitleScene::Go,
		& StartTitleScene::UIScaleDown,
	};

private:

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class StartMainSceneState {
		Transition,
		//UIの移動(スケールアップ)
		UIMoveScaleUp,
		//Ready
		Ready,
		//Go!!
		Go,
		//UIの移動(スケールダウン)
		UIScaleDown,
		//プレイシーンへ
		ChechTempo,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	StartMainSceneState currentState_ = StartMainSceneState::Transition;


};

