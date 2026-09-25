#pragma once

/**
 * @file TitleScene.h
 * @brief タイトルシーン
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
#include <AnimationModel.h>
#include <Dissolve.h>
#include <TitleScene/BaseTitleScene.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia {
	
	/// <summary>
	/// レベルエディタ
	/// </summary>
	class LevelDataManager;

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
/// タイトルシーン
/// </summary>
class TitleScene : public Elysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

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
	~TitleScene() = default;

public:
	/// <summary>
	/// ゲーム管理クラスを設定
	/// </summary>
	/// <param name="gameManager"></param>
	void SetGameManager(Elysia::GameSceneManager* gameManager) override {
		this->gameManager_ = gameManager;
	}

private:
	//入力
	Elysia::Input* input_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//アニメーション管理クラス
	Elysia::AnimationManager* animationManager_ = nullptr;
	//ゲーム管理クラス
	Elysia::GameSceneManager* gameManager_ = nullptr;

private:
	//スピード
	const float_t SPEED = 0.1f;
	//斜め補正
	const float_t DIAGONAL_SCALE_ = 0.7f;
	const float_t SIZE = 1.0f;
	const Vector3 CUBE_SIZE = { .x = SIZE ,.y = SIZE ,.z = SIZE };
private:

	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material playerMaterial_ = {};


	//各シーン
	std::unique_ptr<BaseTitleScene>baseTitleScene_ = nullptr;

};