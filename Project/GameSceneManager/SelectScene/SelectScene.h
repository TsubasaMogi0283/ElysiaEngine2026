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
#include "BaseSelectScene.h"

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
	class MiniAudio;

	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class WindowsSetup;

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
	/// <param name="gameSceneManager">ゲームシーン管理</param>
	inline void SetGameManager(Elysia::GameSceneManager* gameSceneManager) override {
		this->gameSceneManager_ = gameSceneManager;
	}

	/// <summary>
	/// ゲーム管理クラスを取得
	/// </summary>
	/// <returns>ゲーム管理クラス</returns>
	inline Elysia::GameSceneManager* GetGameSceneManager()const {
		return gameSceneManager_;
	}

	/// <summary>
	/// 選択シーンの変更
	/// </summary>
	/// <param name="newSelectScene">新しい選択シーン</param>
	inline void ChangeSelectScene(std::unique_ptr<BaseSelectScene> newSelectScene) {
		if (baseSelectScene_ != newSelectScene) {
			//セット
			baseSelectScene_ = std::move(newSelectScene);
			//初期化
			baseSelectScene_->SetSelectScene(this);
			baseSelectScene_->Initialize();
		}
	}

	/// <summary>
	/// 終了設定
	/// </summary>
	inline void SetIsEnd() {
		this->isEnd_ = true;
	}

	/// <summary>
	/// 全ての譜面データを取得
	/// </summary>
	/// <returns>譜面データ</returns>
	inline std::vector<ExistMusicScoreData> GetExistMusicScoreData()const {
		return allScoreData_;
	}



	inline void SetCurrentDifficulty(const Difficulty& difficulty) {
		this->currentDifficultyNumber_ = difficulty;
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
	Elysia::MiniAudio* miniAudio_ = nullptr;
	//ウィンドウ
	Elysia::WindowsSetup* windowsSetup_ = nullptr;

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

	//決定
	std::unique_ptr<Elysia::Sprite>decideSprite_ = nullptr;
	//戻る
	std::unique_ptr<Elysia::Sprite>backSprite_ = nullptr;


	//全ての譜面データ
	std::vector<ExistMusicScoreData> allScoreData_ = {};
	//トランジェントが開いているか
	bool isOpendTransiont_ = false;
	//現在選択している楽曲の番号
	uint8_t currentSelectedMusicNumber_ = 0u;
	//現在設定されている難易度
	Difficulty currentDifficultyNumber_ = Difficulty::Normal;
	//選択した後の待機時間
	float_t waitingTime_ = 0.0f;
	//終了
	bool isEnd_ = false;

	uint32_t playingMusicNumber_ = 0u;
	int32_t decideSEHandle_ = 0u;
	int32_t selectSEHandle_ = 0u;

	//各クラス
	std::unique_ptr<BaseSelectScene> baseSelectScene_ = nullptr;

};
