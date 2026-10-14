#pragma once

/**
 * @file EndTitleScene.h
 * @brief 終了タイトルシーン
 * @author 茂木翼
 */

#include <TitleScene/BaseTitleScene.h>

/// <summary>
/// タイトル表示クラス
/// </summary>
class EndTitleScene :public BaseTitleScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EndTitleScene() = default;

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
	~EndTitleScene() = default;

private:


};

