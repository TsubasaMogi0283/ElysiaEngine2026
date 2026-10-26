#pragma once

/**
 * @file PlayMainScene.h
 * @brief プレイメインシーンのクラス
 * @author 茂木翼
 */

#include <vector>

#include <MainScene/BaseMainScene.h>
#include <Note/BaseNote.h>
#include <ScoreData/MusicScoreData.h>
#include <ScoreData/MusicInformation.h>
#include <Note/NoteJudgement.h>
#include <Note/NormalTap/NormalTapNote.h>
#include <Note/Long/HighPass/HighPassLongNote.h>
#include <Note/Long/LowPass/LowPassLongNote.h>
#include <Note/Long/TranceGateEighth/TranceGateEighthLongNote.h>
#include <Note/Long/TranceGateSixteenth/TranceGateSixteenthLongNote.h>

#include <Note/Long/BaseLongNote.h>
#include <PauseAsset/PauseAsset.h>
#include <JudgementLine/JudgementLine.h>
#include <Particle3D.h>


/// <summary>
/// メインシーンのプレイシーン
/// </summary>
class PlayMainScene :public BaseMainScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayMainScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mainScene">メインシーン</param>
	void Update()override;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight)override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayMainScene()override = default;

private:

	/// <summary>
	/// レーンの状態
	/// </summary>
	struct LaneCondition {
		//入力されたかどうか
		bool isHit = false;
		//ロングノーツが入力されたかどうか
		bool isHoldLongNote = false;
		//タッチ時間
		float_t touchTime = 0.0f;

		//ホールド状態
		bool isHold = false;
	};

	/// <summary>
	/// 判定UI
	/// </summary>
	struct JudgementUI {
		//スプライト
		std::unique_ptr<Elysia::Sprite>sprite = nullptr;
		//動き
		float_t ratio = 0.0f;
		//時間
		float_t currentTime = 0.0f;

		//使用中かどうか
		bool isUse = false;
		//場所
		uint8_t place;

	};

private:
	/// <summary>
	/// ノーツの流れる処理
	/// </summary>
	/// <param name="noteInformations"></param>
	/// <param name="laneCondition"></param>
	void NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition);

	/// <summary>
	/// ポーズ処理
	/// </summary>
	void Pause();

	/// <summary>
	/// 止める処理
	/// </summary>
	void Stop();

	/// <summary>
	/// 入力処理
	/// </summary>
	/// <param name="laneCondition">レーンの状態</param>
	/// <param name="inputLeft">左入力</param>
	/// <param name="inputRight">右入力</param>
	void Touch(LaneCondition& laneCondition, const uint8_t& inputLeft, const uint8_t inputRight);

private:
	
	//初期の座標
	const Vector2<float_t> NOTE_INITIAL_POSITION_ = { .x = 50.0f,.y = 0.0f };
	static const uint8_t LANE_SIZE_ = static_cast<uint8_t>(NoteLane::Place::Size);
	//レーンのY座標
	const std::array<float_t, LANE_SIZE_> LANE_POSITION_Y_ = {15.0f,5.0f};
	//レーンのZ座標
	const float_t LANE_POSITION_Z_ = 0.0f;

	//ノーマルタップノーツ格納数の最大サイズ
	static const uint8_t NORMAL_NOTE_AMOUNT_ = 64u;
	//ロングノーツ(ハイパス)格納数の最大サイズ
	static const uint8_t HI_PASS_LONG_NOTE_AMOUNT_ = 16u;
	//ロングノーツ(ローパス)格納数の最大サイズ
	static const uint8_t LOW_PASS_LONG_NOTE_AMOUNT_ = 16u;
	//ロングノーツ(トランスゲート8)格納数の最大サイズ
	static const uint8_t TRANS_GATE_EIGHTH_LONG_NOTE_AMOUNT_ = 16u;
	//ロングノーツ(トランスゲート16)格納数の最大サイズ
	static const uint8_t TRANS_GATE_SIXTEENTH_LONG_NOTE_AMOUNT_ = 16u;

	//判定のUI
	//パーフェクト
	static const uint8_t PERFECT_UI_AMOUNT_ = 8u;
	//グレート
	static const uint8_t GREAT_UI_AMOUNT_ = 8u;
	//グッド
	static const uint8_t GOOD_UI_AMOUNT_ = 8u;
	//ミス
	static const uint8_t MISS_UI_AMOUNT_ = 8u;

	//判定の出る時間
	const float_t APPEAR_TIME_ = 0.7f;
	//判定の移動距離
	const float_t JUDGEMENT_UI_MOVE_DISTANCE_ = 100u;

	//動き始める時間のオフセット
	const float_t NOTE_MOVE_START_TIME_OFFSET_ = 2.0f;
	//初期のコンボボーナス倍率
	const float_t INITIAL_COMBO_BONUS_SCALE_ = 1.0f;

	//UIの初期座標
	const float_t INITIAL_UI_POSITION_Y_ = 100.0f;

	//ロングノーツのスコア増加量
	const uint32_t LONG_NOTE_SCORE_INCREASE_VALUE_ = 100u;
	//パーティクルの数
	static const uint8_t PARTICLE_AMOUNT_ = 2u;

private:
	//一時補間場所からの取得
	//譜面情報
	MusicScoreData musicScoreData_ = {};
	//難易度
	int8_t difficulty_ = 0;
	//ハンドル
	uint32_t musicHandle_ = 0u;
	uint32_t jacketTextureHandle_ = 0u;

	//オブジェクトプールで管理するための変数
	//通常タップノーツの固定配列
	std::array<std::unique_ptr<NormalTapNote>, NORMAL_NOTE_AMOUNT_> normalTapNoteArray_ = {};
	//ロング開始ノーツ
	std::array<std::unique_ptr<HighPassLongNote>, HI_PASS_LONG_NOTE_AMOUNT_> highPassLongNoteArray_ = {};
	//ロング(ローパス)開始ノーツ
	std::array<std::unique_ptr<LowPassLongNote>, LOW_PASS_LONG_NOTE_AMOUNT_> lowPassLongNoteArray_ = {};
	//ロング(トランスゲート8)開始ノーツ
	std::array<std::unique_ptr<TranceGateEighthLongNote>, TRANS_GATE_EIGHTH_LONG_NOTE_AMOUNT_> transGateEighthLongNoteArray_ = {};
	//ロング(トランスゲート16)開始ノーツ
	std::array<std::unique_ptr<TranceGateSixteenthLongNote>, TRANS_GATE_SIXTEENTH_LONG_NOTE_AMOUNT_> transGateSixteenthLongNoteArray_ = {};


	//判定のUI
	//Perfect
	std::array<JudgementUI, PERFECT_UI_AMOUNT_ >perfectJudgementUIArray_;
	//Great
	std::array<JudgementUI, GREAT_UI_AMOUNT_ >greatJudgementUIArray_;
	//Good
	std::array<JudgementUI, GOOD_UI_AMOUNT_ >goodJudgementUIArray_;
	//Miss
	std::array<JudgementUI, MISS_UI_AMOUNT_ >missJudgementUIArray_;

	//ポーズアセット
	std::unique_ptr<PauseAsset> pauseAsset_ = nullptr;

	//演奏中
	bool isPlay_ = true;
	//ポーズ中
	bool isPause_ = false;
	//再生時間
	float_t musicTime_ = 0.0f;
	//楽曲の長さ
	float_t musicLength_ = 0.0f;
	//止めた時間
	float_t stopTime_ = 0.0f;

	//コンボのボーナス倍率
	float_t comboBonusScale_ = INITIAL_COMBO_BONUS_SCALE_;

	//レーンの状態
	LaneCondition upLaneCondition = {};
	LaneCondition downLaneCondition = {};

	//記録
	NoteJudgement::Record record_ = {};

	//判定のパーティクル
	std::array<std::unique_ptr<Elysia::Particle3D>, PARTICLE_AMOUNT_> perfectParticleArray_ = {};
	std::array<std::unique_ptr<Elysia::Particle3D>, PARTICLE_AMOUNT_> greatParticleArray_ = {};
	std::array<std::unique_ptr<Elysia::Particle3D>, PARTICLE_AMOUNT_> goodParticleArray_ = {};
	
	//ハンドル
	uint32_t perfectParticleTextureHandle_ = 0u;
	uint32_t greatPartucleTextureHandle_ = 0u;
	uint32_t goodPartucleTextureHandle_ = 0u;
	
};