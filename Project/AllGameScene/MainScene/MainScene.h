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
#include <Sprite.h>
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
	/// <param name="gameManager">ゲーム管理クラス</param>
	inline void SetGameManager(Elysia::GameManager* gameManager) override{
		this->gameManager_ = gameManager;
	}

	/// <summary>
	/// ゲーム管理クラスを取得
	/// </summary>
	/// <returns>ゲーム管理クラス</returns>
	inline Elysia::GameManager* GetGameManager()const {
		return gameManager_;
	}

	/// <summary>
	/// 楽曲情報を取得
	/// </summary>
	/// <returns>楽曲情報</returns>
	inline MusicScoreData GetScoreData()const {
		return musicScoreData_;
	}

	/// <summary>
	/// ハイスピの設定
	/// </summary>
	/// <param name="hiSpeed">ハイスピ</param>
	inline void SetHiSpeed(const float_t& hiSpeed) {
		this->hiSpeed_ = hiSpeed;
	}

	/// <summary>
	/// ハイスピの取得
	/// </summary>
	/// <returns>ハイスピ</returns>
	inline float_t GetHiSpeed()const {
		return hiSpeed_;
	}


private:
	/// <summary>
	/// ノーツ生成
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
	const float_t START_OFFSET_TIME_ = 1.0f;
	//スコアの桁数
	static const uint8_t SCORE_DIGIT_ = 7u;
	//コンボの桁数
	static const uint8_t COMBO_DIGIT_ = 4u;

private:
	/// <summary>
	/// UI情報
	/// </summary>
	struct UIInformation {
		//UI用のスプライト
		std::unique_ptr<Elysia::Sprite>sprite = nullptr;
		//スプライトの座標
		Vector2 position = {};
		//値
		uint16_t value = 0u;
	};


public:
	/// <summary>
	/// ゲージの座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetGaugePosition(const Vector2& position) {
		this->gauge_.position = position;
	}

	/// <summary>
	/// ゲージの座標を取得
	/// </summary>
	/// <returns>ゲージの座標</returns>
	inline Vector2 GetGaugePosition()const {
		return gauge_.position;
	}

	/// <summary>
	/// スコアの桁数を取得
	/// </summary>
	/// <returns>桁数</returns>
	inline uint8_t GetScoreDigit()const {
		return SCORE_DIGIT_;
	}

	/// <summary>
	/// スコアの座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetScorePositions(const std::array<Vector2, SCORE_DIGIT_>& positions) {
		for (uint8_t i = 0u; i < SCORE_DIGIT_; i++) {
			this->scoreArray_[i].position = positions[i];
		}
	}

	/// <summary>
	/// スコアの座標を取得
	/// </summary>
	/// <returns>座標</returns>
	inline  std::array<Vector2, SCORE_DIGIT_> GetScorePositions()const {
		std::array<Vector2, SCORE_DIGIT_> positionArray = {};
		for (uint8_t i = 0u; i < SCORE_DIGIT_; i++) {
			positionArray[i] = this->scoreArray_[i].position;
		}
		return positionArray;
	}

	/// <summary>
	/// コンボの桁数を取得
	/// </summary>
	/// <returns>桁数</returns>
	inline uint8_t GetComboDigit()const {
		return COMBO_DIGIT_;
	}

	/// <summary>
	/// コンボの座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetComboPositions(const std::array<Vector2, COMBO_DIGIT_>& positions) {
		for (uint8_t i = 0u; i < COMBO_DIGIT_; i++) {
			this->comboArray_[i].position = positions[i];
		}
	}

	/// <summary>
	/// コンボの座標を取得
	/// </summary>
	/// <returns>座標</returns>
	inline  std::array<Vector2, COMBO_DIGIT_> GetComboPositions()const {
		std::array<Vector2, COMBO_DIGIT_> positionArray = {};
		for (uint8_t i = 0u; i < COMBO_DIGIT_; i++) {
			positionArray[i] = this->comboArray_[i].position;
		}
		return positionArray;
	}


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
	MusicScoreData musicScoreData_ = {};
	//ハイスピ
	float_t hiSpeed_ = 5.0f;

private:
	//メインシーン共通部分
	//UI
	//ゲージ
	UIInformation gauge_ = {};
	//スコア
	std::array<UIInformation, SCORE_DIGIT_>scoreArray_ = {};
	//コンボ
	std::array<UIInformation, COMBO_DIGIT_>comboArray_ = {};
	
};