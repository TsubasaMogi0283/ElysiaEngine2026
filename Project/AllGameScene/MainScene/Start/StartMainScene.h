#pragma once

/**
 * @file StartMainScene.h
 * @brief スタートメインシーンのクラス
 * @author 茂木翼
 */


#include <MainScene/BaseMainScene.h>

/// <summary>
/// メインシーンのスタートシーン
/// </summary>
class StartMainScene :public BaseMainScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartMainScene();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gameManager">ゲーム管理クラス</param>
	/// <param name="mainScene">メインシーン</param>
	void Initialize(Elysia::GameManager* gameManager, MainScene* mainScene)override;

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
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight)override;

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
	~StartMainScene()override = default;


};

