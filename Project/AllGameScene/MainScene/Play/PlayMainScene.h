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
	void Judge(NoteInformation& noteInformation, LaneCondition& laneCondition, const int32_t& closestNoteIndex = -1);

	/// <summary>
	/// ポーズ処理
	/// </summary>
	void Pause();

	/// <summary>
	/// 再生の再開
	/// </summary>
	void Restart();

private:
	
	//判定のX座標
	const float_t JUDGEMENT_POSITION_X_ = -10.0f;
	//初期のX座標
	const float_t INITIAL_POSITION_X_ = 20.0f;
	//レーンのY座標
	const std::array<float_t, NoteLane::Place::Size> LANE_POSITION_Y_ = { 5.0f,-5.0f };
	//レーンのZ座標
	const float_t LANE_POSITION_Z_ = 0.0f;


private:

	//動き始める時間のオフセット
	const float_t NOTE_MOVE_START_TIME_OFFSET_ = 2.0f;
	//初期のコンボボーナス倍率
	const float_t INITIAL_COMBO_BONUS_SCALE_ = 1.0f;

	//ポーズ時間
	const float_t PAUSE_TIME_ = 3.0f;

	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;

	//ノーマルタップノーツ格納数の最大サイズ
	static const uint32_t NORMAL_NOTEMAX_SIZE_ = 32u;



private:

	//譜面情報
	MusicScoreData musicScoreData_ = {};

	//オブジェクトプールで管理するための通常タップノーツのベクター
	std::array<std::unique_ptr<NormalTapNote>, NORMAL_NOTEMAX_SIZE_> normalTapNoteArray_ = {};

	//演奏中
	bool isPlay_ = true;
	//ポーズ中
	bool isPause_ = false;
	//再生時間
	float_t musicTime_ = 0.0f;
	//楽曲の長さ
	float_t musicLength_ = 0.0f;

	//コンボのボーナス倍率
	float_t comboBonusScale_ = INITIAL_COMBO_BONUS_SCALE_;

	//ポーズから再開したときの時間
	float_t pauseTime_ = PAUSE_TIME_;
	//再開するかどうか
	bool isRestart_ = false;

	//レーンの状態
	LaneCondition upLaneCondition = {};
	LaneCondition downLaneCondition = {};

	//記録
	NoteJudgement::Record record_ = {};
};