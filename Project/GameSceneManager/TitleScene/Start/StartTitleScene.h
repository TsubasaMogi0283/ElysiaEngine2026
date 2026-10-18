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
	};

private:

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class StartTitleSceneState {
		//開けるトランジション
		OpenTransition,
		//テキストの移動
		TextMove,
		//キャラクターの登場
		ApperCharacter,
		
		//この列挙体の量
		Amount,
	};

	//現在の状態
	StartTitleSceneState currentState_ = StartTitleSceneState::OpenTransition;


private:
	//次の状態への待機時間
	const float_t NEXT_WAIT_TIME_ = 1.0f;
	//透明度変化の時間
	const float_t MAX_TRANSPARENCY_TIME_ = 1.0f;

private:

	//待機時間
	std::array<float_t, static_cast<size_t>(StartTitleSceneState::Amount)>waitingTimeArray_ = {};

	//選択の透明度
	float_t selectTransparency_ = 0.0f;

	//時間
	float_t transparencyTime_ = 0.0f;

	//トランジション終了したかどうか
	bool isEndTransition_ = false;

	//
	bool isEndGameTitleTextMove_ = false;
};

