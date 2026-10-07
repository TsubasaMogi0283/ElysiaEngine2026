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
#include <Note/NoteJudgement.h>
#include <Gauge/Gauge.h>

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

	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class WindowsSetup;

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
	//ウィンドウクラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;

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

	//最小値
	const uint16_t MIN_VALUE_ = 0u;
	//ゲージ
	const float_t GAUGE_MIN_RATIO_ = 0.0f;
	const float_t GAUGE_OFFSET_SCALE_ = 0.3f;

	//ゲージの最大サイズ
	Vector2<float_t> GAUGE_MAX_SCALE_ = { .x = 0.7f,.y = 1.0f };
	//スコアの座標のオフセット
	const int32_t SCORE_POSITION_OFFSET_X_ = 50;

	//判定のテクスチャのスケール
	const float_t JUDGEMENT_TEXTURE_SCALE_ = 0.8f;

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

	//リザルト数値などの背景用
	std::unique_ptr<Elysia::Sprite> baseSprite_ = nullptr;
	//楽曲名のスプライト
	std::unique_ptr<Elysia::Sprite> musicTitleSprite_ = nullptr;
	//作曲者名のスプライト
	std::unique_ptr<Elysia::Sprite> musicComposerSprite_ = nullptr;
	//ゲージ
	std::unique_ptr<Gauge> gauge_ = nullptr;
	//メインのスケール
	Vector2<float_t> gaugeMainScale_ = {};
	
	//判定の各桁の情報
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> perfectDigit_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> greatDigit_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> goodDigit_ = {};
	std::array<ValueDigitInformation, ONE_HUNDRED_DIGIT_> missDigit_ = {};
	//テクスチャ
	std::array<std::unique_ptr<Elysia::Sprite>, 4u>judgementSpriteArray_ = {};
	
	//スコア
	std::array<ValueDigitInformation, ONE_MILLION_DIGIT_> score_ = {};
	float_t scoreScale_ = 1.0f;
	//最大コンボ数
	std::array<ValueDigitInformation, ONE_THOUSAND_DIGIT_> maxCombo_ = {};
	
	//数字のテクスチャハンドル
	std::array<uint32_t, NUMBER_AMOUNT_> numberTextureHandle_ = {};
	//数のスケール
	float_t numberScale_ = 1.0f;

	//受け取り用
	MusicInformation musicInformation_ = {};
	NoteJudgement::Record temporaryRecievedRecord_ = {};
	NoteJudgement::Record record_ = {};

	//処理終了
	bool isEnd_ = false;
};