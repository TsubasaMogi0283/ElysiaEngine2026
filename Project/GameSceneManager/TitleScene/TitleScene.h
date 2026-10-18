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
#include <GameNameObject/GameNameObject.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia {

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
		this->gameSceneManager_ = gameManager;
	}

	/// <summary>
	/// ゲーム管理クラスを取得
	/// </summary>
	/// <returns>ゲーム管理クラス</returns>
	inline Elysia::GameSceneManager* GetGameSceneManager()const {
		return gameSceneManager_;
	}

	/// <summary>
	/// ゲーム名オブジェクトクラスの取得
	/// </summary>
	/// <returns></returns>
	inline GameNameObject* GetGameTextObject()const {
		return gameNameObject_.get();
	}

	/// <summary>
	/// タイトルシーンを変更する
	/// </summary>
	/// <param name="newMainScene">新しいタイトルシーン</param>
	inline void ChangeTitleScene(std::unique_ptr<BaseTitleScene> newMainScene) {
		if (baseTitleScene_ != newMainScene) {
			//新しいシーンをセット
			baseTitleScene_ = std::move(newMainScene);
			//初期化
			baseTitleScene_->SetTitleScene(this);
			baseTitleScene_->Initialize();
		}
	};

	/// <summary>
	/// 処理終了の設定
	/// </summary>
	/// <param name="isProcessEnd">処理終了</param>
	inline void SetIsProcessEnd(const bool& isProcessEnd) {
		this->isProcessEnd_ = isProcessEnd;
	}

	/// <summary>
	/// 矢印の座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetArrowPosition(const Vector3<float_t>& position) {
		this->arrowWorldTransform_.translate = position;
	}

	/// <summary>
	/// 選択の透明度を設定
	/// </summary>
	/// <param name="transparency">透明度</param>
	inline void SetSelectModelTransparency(const float_t& transparency) {
		this->arrowMaterial_.color.w = transparency;
		this->startMaterial_.color.w = transparency;
		this->endMaterial_.color.w = transparency;
	}

	/// <summary>
	/// 開始を選択
	/// </summary>
	inline void SetSelectStart() {
		this->isSelectStart_ = true;
		this->isSelectEnd_ = false;
	}

	/// <summary>
	/// 終了を選択
	/// </summary>
	inline void SetSelectEnd() {
		this->isSelectEnd_ = true;
		this->isSelectStart_ = false;
	}

private:
	//入力
	Elysia::Input* input_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//アニメーション管理クラス
	Elysia::AnimationManager* animationManager_ = nullptr;
	//ゲーム管理クラス
	Elysia::GameSceneManager* gameSceneManager_ = nullptr;

private:
	
	//選択のY座標
	const float_t SELECT_POSITION_Y_ = 1.0f;
	//選択時のスケール
	const float_t SELECTED_SCALE_ = 3.0f;
	//非選択時のスケール
	const float_t NO_SELECTED_SCALE_ = 1.0f;

	//矢印の回転
	const float_t ARROW_ROTATE_AMOUNT_ = std::numbers::pi_v<float_t>/36.0f;
private:

	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	
	//地面モデル
	std::unique_ptr<Elysia::Model>groundModel_ = nullptr;
	WorldTransform groundWorldTransform_ = {};
	Material groundMaterial_ = {};

	//開始
	std::unique_ptr<Elysia::Model> startModel_ = nullptr;
	WorldTransform startWorldTransform_ = {};
	Material startMaterial_ = {};
	bool isSelectStart_ = false;

	//終了
	std::unique_ptr<Elysia::Model> endModel_ = nullptr;
	WorldTransform endWorldTransform_ = {};
	Material endMaterial_ = {};
	bool isSelectEnd_ = false;

	//矢印
	std::unique_ptr<Elysia::Model> arrowModel_ = nullptr;
	WorldTransform arrowWorldTransform_ = {};
	Material arrowMaterial_ = {};
	float_t arrowRotateX_ = 0.0f;
	

	//ゲーム名
	std::unique_ptr<GameNameObject> gameNameObject_ = nullptr;

	//各シーン
	std::unique_ptr<BaseTitleScene>baseTitleScene_ = nullptr;
	//処理終了(基本このクラスではいじらない)
	bool isProcessEnd_ = false;
};