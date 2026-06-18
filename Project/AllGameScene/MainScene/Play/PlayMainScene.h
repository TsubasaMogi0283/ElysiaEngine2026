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
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mainScene">メインシーン</param>
	void Update(MainScene* mainScene)override;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	virtual void DrawObject3D(const Camera& camera, const BaseLight& baseLight) = 0;

	/// <summary>
	/// ポストエフェクト描画前
	/// </summary>
	void PreDrawPostEffect()override;

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	void DrawPostEffect()override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayMainScene() = default;

private:

};

