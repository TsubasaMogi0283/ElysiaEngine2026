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
};

