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
	/// <summary>
	/// 開けるトランジション
	/// </summary>
	void OpenTransition();

	/// <summary>
	/// テキストの移動
	/// </summary>
	void TextMove();

	/// <summary>
	/// キャラクターの登場
	/// </summary>
	void ApperCharacter();

	/// <summary>
	/// スタートするかどうか
	/// </summary>
	void StartSelect();

	/// <summary>
	/// 決定
	/// </summary>
	void Decide();

	/// <summary>
	/// セレクトに行く移動
	/// </summary>
	void ToSelect();

	/// <summary>
	/// ゲームを止める
	/// </summary>
	void QuitGame();

	/// <summary>
	/// 閉めるトランジション
	/// </summary>
	void CloseTransition();

private:
	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (StartTitleScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (StartTitleScene::* functionTable[])() = {
		&StartTitleScene::OpenTransition,
		&StartTitleScene::TextMove,
		&StartTitleScene::ApperCharacter,
		&StartTitleScene::StartSelect,
		&StartTitleScene::Decide,
		&StartTitleScene::ToSelect,
		&StartTitleScene::QuitGame,
		&StartTitleScene::CloseTransition,
	};

private:

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class StartMainSceneState {
		//開けるトランジション
		OpenTransition,
		//テキストの移動
		TextMove,
		//キャラクターの登場
		ApperCharacter,
		//スタートするかどうか
		StartSelect,
		//決定
		Decide,
		//セレクトに行く移動
		ToSelect,
		//ゲームを止める
		QuitGame,
		//閉めるトランジション
		CloseTransition,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	StartMainSceneState currentState_ = StartMainSceneState::OpenTransition;


private:
	const float_t NEXT_WAIT_TIME_ = 1.0f;

private:

	//待機時間
	std::array<float_t, static_cast<size_t>(StartMainSceneState::Amount)>waitingTimeArray_ = {};

	//トランジション終了したかどうか
	bool isEndTransition_ = false;
};

