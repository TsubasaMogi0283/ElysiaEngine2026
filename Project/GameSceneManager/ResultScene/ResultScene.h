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
	/// ゲージの上昇
	/// </summary>
	void IncreaseGauge();

	/// <summary>
	/// 判定結果の増加
	/// </summary>
	void IncreaseJudgement();

	/// <summary>
	/// 最大コンボの増加
	/// </summary>
	void IncreaseMaxCombo();

	/// <summary>
	/// スコアの上昇
	/// </summary>
	void IncreaseScore();

	/// <summary>
	/// ランクの移動
	/// </summary>
	void MoveRank();

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
		&ResultScene::IncreaseGauge,
		&ResultScene::IncreaseJudgement,
		&ResultScene::IncreaseMaxCombo,
		&ResultScene::IncreaseScore,
		&ResultScene::MoveRank,
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
		//ゲージの上昇
		IncreaseGauge,
		//判定結果の増加
		IncreaseJudgement,
		//最大コンボの増加
		IncreaseMaxCombo,
		//スコアの上昇
		IncreaseScore,

		//ランク
		MoveRank,
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
	static const uint8_t ONE_DIGIT_ = 1u;
	//十の桁
	static const uint8_t TEN_DIGIT_ = 2u;
	//百の桁
	static const uint8_t ONE_HUNDRED_DIGIT_ = 3u;
	//千の桁
	static const uint8_t ONE_THOUSAND_DIGIT_ = 4u;
	//一万の桁
	static const uint8_t TEN_THOUSAND_DIGIT_ = 5u;
	//十万の桁
	static const uint8_t ONE_HUNDRED_THOUSAND_DIGIT_ = 6u;
	//百万の桁
	static const uint8_t ONE_MILLION_DIGIT_ = 7u;

	//数字の数
	static const uint8_t NUMBER_AMOUNT_ = 10u;

	//最小値
	const uint16_t MIN_VALUE_ = 0u;
	//ゲージ
	const float_t GAUGE_MIN_RATIO_ = 0.0f;
	const float_t GAUGE_OFFSET_SCALE_ = 0.3f;
	const float_t MAX_GAUGE_INCREASE_TIME_ = 1.5f;

	//ゲージの最大サイズ
	const Vector2<float_t> GAUGE_MAX_SCALE_ = { .x = 0.7f,.y = 1.0f };
	//スコアの座標のオフセット
	const int32_t SCORE_POSITION_OFFSET_X_ = 50;

	//判定のテクスチャのスケール
	const float_t JUDGEMENT_TEXTURE_SCALE_ = 0.65f;
	const float_t MAX_JUDGEMENT_INCREASE_TIME_ = 1.0f;

	//コンボ
	const float_t MAX_COMBO_INCREASE_TIME_ = 0.5f;
	//スケール
	const float_t MAX_COMBO_SCALE_ = 0.5f;
	//スケールダウンの時間
	const float_t MAX_SCALE_DOWN_TIME_ = 1.0f;

	//ランク
	const float_t RANK_SCALE_ = 0.5f;


	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;

private:
	/// <summary>
	/// 値の桁情報
	/// </summary>
	struct ValueDigitInformation {
		//スプライト
		std::unique_ptr<Elysia::Sprite> sprite;
		//スプライトの座標
		Vector2<float_t> position;
		//値
		uint8_t value;
		//テクスチャハンドル
		uint32_t textureHandle;
	};

	/// <summary>
	/// 判定の情報
	/// </summary>
	struct JudgementInformation {
		//各桁の情報
		std::array<ValueDigitInformation, ONE_THOUSAND_DIGIT_> valueDigitInformationArray_ = {};
		//テキスト
		std::unique_ptr<Elysia::Sprite> textSprite = nullptr;
		//増加開始時間
		float_t increaseStartTime = 0.0f;
		//増加終了時間
		float_t increaseEndTime = 0.0f;
		//値
		uint16_t value = 0u;

	};

	/// <summary>
	/// ランク
	/// </summary>
	enum class RankSelection {
		//AllPerfect
		P,
		//FullCombo
		F,
		//Sランク
		S,
		//Aランク
		A,
		//Bランク
		B,
		//Cランク
		C,
		//Dランク
		D,

		//量
		Size,
	};

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

	//リザルト数値などの背景用
	std::unique_ptr<Elysia::Sprite> baseSprite_ = nullptr;
	//楽曲名のスプライト
	std::unique_ptr<Elysia::Sprite> musicTitleSprite_ = nullptr;
	//作曲者名のスプライト
	std::unique_ptr<Elysia::Sprite> musicComposerSprite_ = nullptr;
	//ゲージ
	std::unique_ptr<Gauge> gauge_ = nullptr;
	//上昇時間
	float_t gaugeIncreaseTime_ = 0.0f;

	//判定の情報
	std::array<JudgementInformation, static_cast<uint8_t>(NoteJudgement::Selection::Size)>judgementInformation_ = {};
	float_t judgementIncreaseTime = 0.0f;
	bool isEndIncreaseJudgement_ = false;

	//最大コンボ数
	std::array<ValueDigitInformation, ONE_THOUSAND_DIGIT_> maxComboDigit_ = {};
	std::unique_ptr<Elysia::Sprite>maxComboSprite_ = nullptr;
	float_t increaseMaxComboTime_ = 0.0f;
	//スコア
	std::array<ValueDigitInformation, ONE_MILLION_DIGIT_> scoreDigit_ = {};
	float_t scoreScale_ = 1.0f;
	float_t scoreIncreaseTime_ = 0.0f;
	//ランク
	std::unique_ptr<Elysia::Sprite>rankSprite_ = nullptr;
	float_t rankScaleDownTime_ = 0.0f;


	//数字のテクスチャハンドル
	std::array<uint32_t, NUMBER_AMOUNT_> numberTextureHandle_ = {};
	//数のスケール
	float_t numberScale_ = 1.0f;

	//受け取り用
	MusicScoreData musicScoreData_ = {};
	NoteJudgement::Record temporaryRecievedRecord_ = {};
	
	//決定
	std::unique_ptr<Elysia::Sprite>decideSprite_ = nullptr;

	//処理終了
	bool isEnd_ = false;
};