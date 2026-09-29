#pragma once

/**
 * @file DisplayTitleScene.h
 * @brief タイトル表示シーン
 * @author 茂木翼
 */

#include <TitleScene/BaseTitleScene.h>

/// <summary>
/// タイトル表示クラス
/// </summary>
class DisplayTitleScene :public BaseTitleScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DisplayTitleScene() = default;

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

	/// <summary>
	/// ゲームを止める
	/// </summary>
	void QuitGame();
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
		&DisplayTitleScene::QuitGame,
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
		//ゲームを止める
		QuitGame,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	DisplayTitleSceneState currentState_ = DisplayTitleSceneState::StartSelect;


};

