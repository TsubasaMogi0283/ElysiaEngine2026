#pragma once

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
/// 結果シーン
/// </summary>
class ResultScene : public Elysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene();

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
	~ResultScene() = default;

public:

	/// <summary>
	/// ゲーム管理クラスを設定
	/// </summary>
	/// <param name="gameManager"></param>
	void SetGameManager(Elysia::GameSceneManager* gameManager) override {
		this->gameSceneManager_ = gameManager;
	}


private:

	/// <summary>
	/// トランジション
	/// </summary>
	void Transition();

	/// <summary>
	/// 値が増えていく
	/// </summary>
	void IncreaseValue();


private:

	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (ResultScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (ResultScene::* functionTable[])() = {
		&ResultScene::Transition,
		& ResultScene::IncreaseValue,
	};

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class ResultSceneState {
		//トランジション
		Transition,
		//値が増えていく
		IncreaseValue,


		//この列挙体の量
		Amount,
	};

	//現在の状態
	ResultSceneState currentState_ = ResultSceneState::Transition;

private:
	/// <summary>
	/// 数値UI情報
	/// </summary>
	struct ValueUIInformation {
		//スプライト
		std::unique_ptr<Elysia::Sprite> sprite = nullptr;
		//値
		uint16_t value;
	};

private:
	//入力
	Elysia::Input* input_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//ゲーム管理クラス
	Elysia::GameSceneManager* gameSceneManager_ = nullptr;

private:

	//一の桁
	static const uint8_t ONE_DIGIT_ = 0u;
	//十の桁
	static const uint8_t TEN_DIGIT_ = 1u;
	//百の桁
	static const uint8_t ONE_HUNDRED_DIGIT_ = 2u;
	//千の桁
	static const uint8_t ONE_THOUSAND_DIGIT_ = 3u;
	//一万の桁
	static const uint8_t TEN_THOUSAND_DIGIT_ = 4u;
	//十万の桁
	static const uint8_t ONE_HUNDRED_THOUSAND_DIGIT_ = 5u;
	//百万の桁
	static const uint8_t ONE_MILLION_DIGIT_ = 6u;

private:

	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material playerMaterial_ = {};


	//楽曲名のスプライト
	std::unique_ptr<Elysia::Sprite> musicTitleSprite_ = nullptr;
	//作曲者名のスプライト
	std::unique_ptr<Elysia::Sprite> musicComposerSprite_ = nullptr;

	//判定
	std::array<ValueUIInformation, ONE_HUNDRED_DIGIT_>perfect_ = {};
	std::array<ValueUIInformation, ONE_HUNDRED_DIGIT_> great_ = {};
	std::array<ValueUIInformation, ONE_HUNDRED_DIGIT_> good_ = {};
	std::array<ValueUIInformation, ONE_HUNDRED_DIGIT_> miss_ = {};

	//スコア
	std::array<ValueUIInformation, ONE_MILLION_DIGIT_> score_ = {};
	//最大コンボ数
	std::array<ValueUIInformation, ONE_THOUSAND_DIGIT_> maxCombo_ = {};

	//一時取得場所
	MusicInformation musicInformation_ = {};


};