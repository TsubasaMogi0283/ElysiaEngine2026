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
	~EndTitleScene() = default;

private:



};

