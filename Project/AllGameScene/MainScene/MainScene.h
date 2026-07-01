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
#include "WorldTransform.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "BaseMainScene.h"
#include <Audio.h>
#include <ScoreData/MusicScoreData.h>

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
	/// メインシーンを変更する
	/// </summary>
	/// <param name="newMainScene">新しいメインシーン</param>
	inline void ChangeMainScene(std::unique_ptr<BaseMainScene> newMainScene) {
		if (baseMainScene_ != newMainScene) {
			//新しいシーンをセット
			baseMainScene_ = std::move(newMainScene);
			//初期化
			baseMainScene_->SetMainScene(this);
			baseMainScene_->Initialize();
		}
	};

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
	/// 楽曲情報を取得
	/// </summary>
	/// <returns></returns>
	inline MusicScoreData GetScoreData()const {
		return musicInformation;
	}

	/// <summary>
	/// ハイスピの設定
	/// </summary>
	/// <param name="hiSpeed"></param>
	inline void SetHiSpeed(const float_t& hiSpeed) {
		this->hiSpeed_ = hiSpeed;
	}

	/// <summary>
	/// ハイスピの取得
	/// </summary>
	/// <returns></returns>
	inline float_t GetHiSpeed()const {
		return hiSpeed_;
	}

private:
	/// <summary>
	/// ノーツ生成クラス
	/// </summary>
	void GenerateNotes();
	
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
	//開始オフセット
	const float_t START_OFFSET_TIME_ = 2.0f;
	//判定のX座標
	const float_t JUDGEMENT_POSITION_X_ = -10.0f;
	//初期のX座標
	const float_t INITIAL_POSITION_X_ = 40.0f;
	//レーンのY座標
	const std::array<float_t, NoteLane::Place::Size> LANE_POSITION_Y_ = { 10.0f,-10.0f };
	//レーンのZ座標
	const float_t LANE_POSITION_Z_ = 0.0f;

private:
	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;
	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//メインシーンの中で細かく分けるための変数
	std::unique_ptr<BaseMainScene> baseMainScene_ = nullptr;

	//楽曲情報
	MusicInformation musicInformation_ = {};
	//譜面データ
	MusicScoreData musicInformation = {};
	//ハイスピ
	float_t hiSpeed_ = 5.0f;


};