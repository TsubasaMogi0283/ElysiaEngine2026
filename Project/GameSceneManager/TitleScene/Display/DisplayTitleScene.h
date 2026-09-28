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




};

