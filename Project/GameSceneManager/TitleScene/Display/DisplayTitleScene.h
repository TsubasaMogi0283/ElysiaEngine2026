#pragma once

/**
 * @file DisplayTitleScene.h
 * @brief タイトル表示シーン
 * @author 茂木翼
 */

#include <TitleScene/BaseTitleScene.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia {

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// 入力
	/// </summary>
	class Input;
}

/// <summary>
/// タイトル表示クラス
/// </summary>
class DisplayTitleScene :public BaseTitleScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DisplayTitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DisplayTitleScene() = default;

private:

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

private:
	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (DisplayTitleScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (DisplayTitleScene::* functionTable[])() = {
		&DisplayTitleScene::StartSelect,
		&DisplayTitleScene::Decide,
		&DisplayTitleScene::ToSelect,
	};


	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class DisplayTitleSceneState {
		//スタートするかどうか
		StartSelect,
		//決定
		Decide,
		//セレクトに行く移動
		ToSelect,

		//この列挙体の数
		Size,
	};

	//現在の状態
	DisplayTitleSceneState currentState_ = DisplayTitleSceneState::StartSelect;

private:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//入力
	Elysia::Input* input_ = nullptr;

private:
	//点滅間隔
	const float_t FLASH_INTERVAL_ = 0.5f;
	//高速点滅間隔
	const float_t RAPID_FLASH_TIME_ = 0.05f;
	//最大高速点滅回数
	const uint8_t MAX_RAPID_FLASH_COUNT_ = 10u;
private:
	//SPACEでスタートのスプライト
	std::unique_ptr<Elysia::Sprite> startTextSprite_ = nullptr;

	//点滅時間
	float_t flashTime_ = 0.0f;
	//高速点滅時間
	float_t rapidFlashTime_ = 0.0f;
	uint8_t rapidFlashCount_ = 0u;

};

