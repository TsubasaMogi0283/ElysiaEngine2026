#pragma once

/**
 * @file MainScene.h
 * @brief メインシーンのクラス
 * @author 茂木翼
 */

#include <memory>
#include <array>

#include "IGameScene.h"
#include "BackTexture.h"
#include "Model.h"
#include "Particle3D.h"
#include "WorldTransform.h"
#include "AABB.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include <PointLight.h>
#include <SpotLight.h>
#include <AnimationModel.h>
#include <Dissolve.h>
#include "BaseMainScene.h"
#include <Audio.h>

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

}

/// <summary>
/// メインシーン
/// </summary>
class MainScene : public Elysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MainScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

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
	~MainScene() = default;


public:
	
	/// <summary>
	/// ゲーム管理クラスを設定
	/// </summary>
	/// <param name="gameManager"></param>
	inline void SetGameManager(Elysia::GameManager* gameManager) override{
		this->gameManager_ = gameManager;
	}

	/// <summary>
	/// ゲーム管理クラスを取得
	/// </summary>
	/// <returns></returns>
	inline Elysia::GameManager* GetGameManager()const {
		return gameManager_;
	}

	/// <summary>
	/// メインシーンを変更する
	/// </summary>
	/// <param name="newMainScene">新しいメインシーン</param>
	void ChangeMainScene(std::unique_ptr<BaseMainScene> newMainScene);
	
private:
	//入力
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//ゲーム管理クラス
	Elysia::GameManager* gameManager_ = nullptr;

private:
	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;
	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	PointLight pointLight={};
	SpotLight spotLight = {};
	//メインシーンの中で細かく分けるための変数
	std::unique_ptr<BaseMainScene> baseMainScene_ = nullptr;

};