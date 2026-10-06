#pragma once

#include <memory>
#include <array>

#include "IGameScene.h"
#include "BackTexture.h"
#include <Sprite.h>
#include "Model.h"
#include "WorldTransform.h"
#include <Camera.h>
#include "Material.h"
#include "DirectionalLight.h"
#include <ScoreData/MusicInformation.h>

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
	void Open();

	/// <summary>
	/// 値が増えていく
	/// </summary>
	void IncreaseValue();

	/// <summary>
	/// 表示
	/// </summary>
	void Display();

	/// <summary>
	/// 戻る
	/// </summary>
	void Return();

	/// <summary>
	/// 閉める
	/// </summary>
	void Close();

private:

	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (ResultScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (ResultScene::* functionTable[])() = {
		&ResultScene::Open,
		&ResultScene::IncreaseValue,
		&ResultScene::Display,
		&ResultScene::Return,
		&ResultScene::Close,
	};

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class ResultSceneState {
		//トランジション
		Transition,
		//値が増えていく
		IncreaseValue,
		//表示
		Display,
		//戻る
		Return,
		//閉める
		Close,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	ResultSceneState currentState_ = ResultSceneState::Transition;

private:
	/// <summary>
	/// 数値UI情報
	/// </summary>
	struct ValueDigitInformation {
		//スプライト
		std::unique_ptr<Elysia::Sprite> sprite = nullptr;
		//スプライトの座標
		Vector2<int32_t> position;
		//値
		uint16_t value;
		//テクスチャハンドル
		uint32_t textureHandle;
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
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
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

	//数字の数
	static const uint8_t NUMBER_AMOUNT_ = 10u;

private:
	//各数値の座標オフセット
	//最大コンボ
	Vector2<int32_t> comboOffsetPosition_ = {};

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

	//判定の各桁の情報
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_>perfect_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> great_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> good_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> miss_ = {};
	//結果
	uint16_t perfectResult_ = 0u;
	uint16_t greatResult_ = 0u;
	uint16_t goodResult_ = 0u;
	uint16_t missResult_ = 0u;

	//スコア
	std::array<ValueDigitInformation, ONE_MILLION_DIGIT_> score_ = {};
	uint16_t scoreResult_ = 0u;

	//最大コンボ数
	std::array<ValueDigitInformation, ONE_THOUSAND_DIGIT_> maxCombo_ = {};
	uint16_t maxResultResult_ = 0u;

	//数字のテクスチャハンドル
	std::array<uint32_t, NUMBER_AMOUNT_> numberTextureHandle_ = {};
	//数のスケール
	float_t numberScale_ = 1.0f;

	//一時取得場所
	MusicInformation musicInformation_ = {};
	NoteJudgement::Record record_ = {};

	//処理終了
	bool isEnd_ = false;
};