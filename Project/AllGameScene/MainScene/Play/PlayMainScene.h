#pragma once

/**
 * @file PlayMainScene.h
 * @brief プレイメインシーンのクラス
 * @author 茂木翼
 */

#include <MainScene/BaseMainScene.h>

/// <summary>
/// メインシーンのプレイシーン
/// </summary>
class PlayMainScene :public BaseMainScene{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mainScene">メインシーン</param>
	void Update(MainScene* mainScene);

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D();

	/// <summary>
	/// ポストエフェクト描画前
	/// </summary>
	void PreDrawPostEffect();

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	void DrawPostEffect();

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayMainScene() = default;

private:

};

