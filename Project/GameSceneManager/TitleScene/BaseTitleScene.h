#pragma once

/**
 * @file BaseTitleScene.h
 * @brief 基底タイトルシーン
 * @author 茂木翼
 */

#include <memory>
#include <array>
#include <Sprite.h>

 /// <summary>
 /// ElysiaEngine(前方宣言)
 /// </summary>
namespace Elysia {
	/// <summary>
	/// ウィンドウ
	/// </summary>
	class WindowsSetup;

	/// <summary>
	/// レベルエディタ
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameSceneManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;

	/// <summary>
	/// アニメーション管理クラス
	/// </summary>
	class AnimationManager;

	/// <summary>
	/// 入力クラス
	/// </summary>
	class Input;

	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;

}

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene;

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// カメラ基底クラス
/// </summary>
struct BaseLight;


/// <summary>
/// 基底タイトルシーン
/// </summary>
class BaseTitleScene{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="baseLight">ライト</param>
	virtual void DrawObject3D([[maybe_unused]] const Camera& camera, [[maybe_unused]] const BaseLight& baseLight){};

	/// <summary>
	/// スプライト
	/// </summary>
	virtual void DrawSprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseTitleScene() = default;

public:

	/// <summary>
	/// タイトルシーンの設定を設定
	/// </summary>
	virtual void SetTitleScene(TitleScene* titleScene) {
		this->titleScene_ = titleScene;
	}

protected:
	//タイトルシーン
	TitleScene* titleScene_ = nullptr;
	//ウィンドウの設定
	Elysia::WindowsSetup* windowsSetup_ = nullptr;
	//入力
	Elysia::Input* input_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//テクスチャーハンドル
	Elysia::TextureManager* textureManager_ = nullptr;

protected:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;


};

