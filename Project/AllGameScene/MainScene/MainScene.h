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
class MainScene : public Elysia::IGameScene {
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
	inline void SetGameManager(Elysia::GameManager* gameManager) override {
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

	/// <summary>
	/// 各数値をテクスチャに割り当て
	/// </summary>
	void AssignToTexture();

private:
	//入力
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//ゲーム管理クラス
	Elysia::GameManager* gameManager_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;

private:
	//開始オフセット
	const float_t START_OFFSET_TIME_ = 1.0f;
	//数字のテクスチャの数
	static const uint8_t NUMBER_TEXTURE_AMOUNT_ = 10u;

	//桁関係の定数
	//スコアの桁数
	static const uint8_t SCORE_DIGIT_ = 7u;
	//コンボの桁数
	static const uint8_t COMBO_DIGIT_ = 4u;

	//一の桁
	const uint8_t ONE_DIGIT_ = 0u;
	//十の桁
	const uint8_t TEN_DIGIT_ = 1u;
	//百の桁
	const uint8_t ONE_HUNDRED_DIGIT_ = 2u;
	//千の桁
	const uint8_t ONE_THOUSAND_DIGIT_ = 3u;
	//一万の桁
	const uint8_t TEN_THOUSAND_DIGIT_ = 4u;
	//十万の桁
	const uint8_t ONE_HUNDRED_THOUSAND_DIGIT_ = 5u;
	//百万の桁
	const uint8_t ONE_MILLION_DIGIT_ = 6u;

private:
	//初期のゲージ座標
	Vector2<float_t> initialGaugePosition_ = {};
	//通常表示時のゲージ座標
	Vector2<float_t> gaugeDisplayPosition_ = {};

	//初期のコンボ座標
	float_t initialComboPositionY_ = 0.0f;
	//通常表示時のコンボ座標
	float_t comboDisplayPositionY_ = 0.0f;

	//初期のスコア座標
	std::array<float_t, SCORE_DIGIT_> initialScorePositionXArray_ = {};
	float_t initialScorePositionY_ = 0.0f;
	//通常表示時のスコア座標
	float_t scoreDisplayPositionY_ = 0.0f;
	//スコア座標のオフセット
	float_t scorePositionOffsetX_ = 1000.0f;
	//スコアのスケール
	float_t scoreScale_ = 0.5f;

	Vector2<uint64_t> numberTextureSize_ = { .x = 0u, .y = 0u };

private:
	/// <summary>
	/// UI情報
	/// </summary>
	struct UIInformation {
		//UI用のスプライト
		std::unique_ptr<Elysia::Sprite>sprite = nullptr;
		//スプライトの座標
		Vector2<float_t> position = {};
		//値
		uint16_t value = 0u;
		//テクスチャハンドル
		uint32_t textureHandle = 0u;
	};


public:
	/// <summary>
	/// ゲージの座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetGaugePosition(const Vector2<float_t>& position) {
		this->gauge_.sprite->SetPosition(position);
	}

	/// <summary>
	/// ゲージの通常表示座標を取得
	/// </summary>
	/// <returns>ゲージの通常表示座標</returns>
	inline Vector2<float_t> GetGaugeDisplayPosition()const {
		return gaugeDisplayPosition_;
	}

	/// <summary>
	/// 初期ゲージ座標を取得
	/// </summary>
	/// <returns>初期ゲージ座標</returns>
	inline Vector2<float_t> GetInitialGaugePosition()const {
		return initialGaugePosition_;
	}

	/// <summary>
	/// コンボの初期座標を取得
	/// </summary>
	/// <returns>初期座標</returns>
	inline float_t GetInitialComboPositionY()const {
		return initialComboPositionY_;
	}

	inline float_t GetInitialScorePositionY()const {
		return initialScorePositionY_;
	}

	inline float_t GetScoreDisplayPositionY()const {
		return scoreDisplayPositionY_;
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
	inline void SetScorePositionsY(const float_t positionY) {
		for (uint8_t i = 0u; i < SCORE_DIGIT_; i++) {
			this->scoreArray_[i].sprite->SetPosition({initialScorePositionXArray_[i], positionY});
		}
	}


	/// <summary>
	/// コンボの桁数を取得
	/// </summary>
	/// <returns>桁数</returns>
	inline uint8_t GetComboDigit()const {
		return COMBO_DIGIT_;
	}

	/// <summary>
	/// コンボのY座標を設定
	/// </summary>
	/// <param name="positionY">Y座標</param>
	inline void SetComboPositionsY(const float_t& positionY) {
		for (uint8_t i = 0u; i < COMBO_DIGIT_; i++) {
			this->comboArray_[i].position.y = positionY;
		}
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
	//数字のテクスチャハンドル
	std::array<uint32_t, NUMBER_TEXTURE_AMOUNT_>numberTextureHandlesArray = {};
	//ゲージ
	UIInformation gauge_ = {};
	Vector2<float_t> gaugeScale = { .x = 1.0f,.y = 1.0f };
	//スコア
	std::array<UIInformation, SCORE_DIGIT_>scoreArray_ = {};
	uint32_t totalScore_ = 0u;
	//コンボ
	std::array<UIInformation, COMBO_DIGIT_>comboArray_ = {};
	uint16_t totalCombo_ = 0u;
	//最大コンボ
	uint16_t maxCombo_ = 0u;




	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };


};