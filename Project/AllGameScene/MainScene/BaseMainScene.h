#pragma once
/**
 * @file BaseMainScene.h
 * @brief メインシーンの基底クラス
 * @author 茂木翼
 */

#include <memory>
#include <array>


/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia {
	/// <summary>
	/// レベルエディタ
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager;

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
/// メインシーン
/// </summary>
class MainScene;

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// カメラ基底クラス
/// </summary>
struct BaseLight;

/// <summary>
/// メインシーンの基底クラス
/// </summary>
class BaseMainScene{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gameManager"></param>
	/// <param name="mainScene"></param>
	virtual void Initialize(Elysia::GameManager* gameManager,MainScene* mainScene) = 0;
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mainScene">メインシーン</param>
	virtual void Update(MainScene* mainScene) = 0;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	virtual void DrawObject3D(const Camera& camera,const BaseLight& baseLight)=0;

	/// <summary>
	/// ポストエフェクト描画前
	/// </summary>
	virtual void PreDrawPostEffect() = 0;

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	virtual void DrawPostEffect() = 0;

	/// <summary>
	/// スプライト
	/// </summary>
	virtual void DrawSprite() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseMainScene()=default;
public:

	//処理が終わったかどうかを返す
	bool GetIsEnd() const {
		return isEnd_;
	}

protected:
	//ゲーム管理クラス
	Elysia::GameManager* gameManager_ = nullptr;
	//入力
	Elysia::Input* input_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;

protected:
	//処理が終わったかどうか
	bool isEnd_ = false;

};

