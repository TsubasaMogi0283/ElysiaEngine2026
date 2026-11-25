#pragma once

/**
 * @file DifficultySelectScene.h
 * @brief 難易度選択シーンクラス
 * @author 茂木翼
 */


#include <SelectScene/BaseSelectScene.h>

/// <summary>
/// 難易度選択シーン
/// </summary>
class DifficultySelectScene :public BaseSelectScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DifficultySelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

};

