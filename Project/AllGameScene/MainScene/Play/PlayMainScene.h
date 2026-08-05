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
#include <Note/Long/BaseLongNote.h>
#include <PauseAsset/PauseAsset.h>
#include <JudgementLine/JudgementLine.h>

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
		bool isHitLongNote = false;
		//タッチ時間
		float_t touchTime = 0.0f;

		//ホールド状態
		bool isHold = false;
	};

private:
	/// <summary>
	/// ノーツの流れる処理
	/// </summary>
	/// <param name="noteInformations"></param>
	/// <param name="laneCondition"></param>
	void NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition);

	/// <summary>
	/// 判定
	/// </summary>
	/// <param name="noteInformation"></param>
	/// <param name="laneCondition"></param>
	/// <param name="closestNoteIndex"></param>
	void Judge(std::vector<NoteInformation>& noteInformation, LaneCondition& laneCondition, const int32_t& closestNoteIndex);

	/// <summary>
	/// ポーズ処理
	/// </summary>
	void Pause();

	/// <summary>
	/// 止める処理
	/// </summary>
	void Stop();

private:
	
	//初期のX座標
	const float_t INITIAL_POSITION_X_ = 50.0f;
	const float_t INITIAL_POSITION_Y_ = 0.0f;
	static const uint8_t LANE_SIZE_ = static_cast<uint8_t>(NoteLane::Place::Size);
	//レーンのY座標
	const std::array<float_t, LANE_SIZE_> LANE_POSITION_Y_ = {10.0f,0.0f};
	//レーンのZ座標
	const float_t LANE_POSITION_Z_ = 0.0f;

	//ノーマルタップノーツ格納数の最大サイズ
	static const uint8_t NORMAL_NOTE_MAX_SIZE_ = 32u;
	//ロングノーツ格納数の最大サイズ
	static const uint8_t HI_PASS_LONG_NOTE_MAX_SIZE_ = 8u;

	//動き始める時間のオフセット
	const float_t NOTE_MOVE_START_TIME_OFFSET_ = 2.0f;
	//初期のコンボボーナス倍率
	const float_t INITIAL_COMBO_BONUS_SCALE_ = 1.0f;

	
private:

	//譜面情報
	MusicScoreData musicScoreData_ = {};

	//オブジェクトプールで管理するための変数
	//通常タップノーツの固定配列
	std::array<std::shared_ptr<NormalTapNote>, NORMAL_NOTE_MAX_SIZE_> normalTapNoteArray_ = {};
	//ロング開始ノーツ
	std::array<std::unique_ptr<HighPassLongNote>, HI_PASS_LONG_NOTE_MAX_SIZE_> longTapNoteArray_ = {};

	//サンプル
	std::unique_ptr<BaseLongNote> longNoteSmaple_ = nullptr;
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


	//判定線
	std::unique_ptr<JudgementLine> judgementLine_ = nullptr;

	//レーンの状態
	LaneCondition upLaneCondition = {};
	LaneCondition downLaneCondition = {};

	//記録
	NoteJudgement::Record record_ = {};
};