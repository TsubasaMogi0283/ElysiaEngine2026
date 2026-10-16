#pragma once

/**
 * @file SelectScene.h
 * @brief 選択シーンのクラス
 * @author 茂木翼
 */


#include <memory>
#include <vector>

#include "IGameScene.h"
#include "BackTexture.h"
#include <Sprite.h>
#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include <Vector4.h>
#include "Material.h"
#include "DirectionalLight.h"
#include <Jacket/Jacket.h>
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
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;

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
/// 選択シーン
/// </summary>
class SelectScene : public Elysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene();

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
	~SelectScene() = default;

public:
	/// <summary>
	/// ゲーム管理クラスを設定
	/// </summary>
	/// <param name="gameManager"></param>
	void SetGameManager(Elysia::GameSceneManager* gameManager) override {
		this->gameSceneManager_ = gameManager;
	}

private:

	//入力
	Elysia::Input* input_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//ゲーム管理クラス
	Elysia::GameSceneManager* gameSceneManager_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//待機時間
	const float_t WATING_TIME_ = 2.0f;


private:

	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material playerMaterial_ = {};
	//ジャケット
	std::unique_ptr<Jacket>jacket_ = {};

	//全ての譜面データ
	std::vector<MusicScoreData> allScoreData_ = {};

	bool isOpendTransiont_ = false;
	//現在選択している楽曲の番号
	uint8_t currentSelectedMusicNumber_ = 0u;
	//現在設定されている難易度
	int8_t currentDifficultyNumber_ = static_cast<int8_t>(Difficulty::Normal);
	//選択した後の待機時間
	float_t waitingTime_ = 0.0f;
	//終了
	bool isEnd_ = false;



	bool isPlay_ = false;

};
