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
#include <JudgementLine/JudgementLine.h>
#include <MainScene/Play/PlayMainScene.h>


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
/// テストシーン
/// </summary>
class TestScene : public Elysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TestScene();

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
	~TestScene() = default;

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

private:
	/// <summary>
	/// ノーツ生成
	/// </summary>
	void GenerateNotes();

	/// <summary>
	/// ノーツの流れる処理
	/// </summary>
	/// <param name="noteInformations"></param>
	/// <param name="laneCondition"></param>
	void NoteFlow(std::vector<NoteInformation>& noteInformations, LaneCondition& laneCondition);

	/// <summary>
	/// 入力処理
	/// </summary>
	/// <param name="laneCondition">レーンの状態</param>
	/// <param name="inputLeft">左入力</param>
	/// <param name="inputRight">右入力</param>
	void Touch(LaneCondition& laneCondition, const uint8_t& inputLeft, const uint8_t inputRight);


private:
	//ゲーム管理クラス
	Elysia::GameSceneManager* gameSceneManager_ = nullptr;

	//入力
	Elysia::Input* input_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//テクスチャーハンドル
	Elysia::TextureManager* textureManager_ = nullptr;
private:
	//ノーマルタップノーツ格納数の最大サイズ
	static const uint8_t NORMAL_NOTE_MAX_SIZE_ = 32u;
	//ロングノーツ(ハイパス)格納数の最大サイズ
	static const uint8_t HI_PASS_LONG_NOTE_MAX_SIZE_ = 6u;
	//ロングノーツ(ローパス)格納数の最大サイズ
	static const uint8_t LOW_PASS_LONG_NOTE_MAX_SIZE_ = 6u;
	//ロングノーツ(トランスゲート8)格納数の最大サイズ
	static const uint8_t TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_ = 6u;
	//ロングノーツ(トランスゲート16)格納数の最大サイズ
	static const uint8_t TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_ = 6u;

	const Vector2<float_t> INITIAL_POSITION_ = { .x = 50.0f,.y = 0.0f };
	//判定のX座標
	const float_t JUDGEMENT_POSITION_X_ = -17.0f;

	//曲の開始時間
	const float_t MUSIC_START_TIME_ = 0.0f;
	//終了時間
	const float_t MUSIC_END_TIME_ = 18.0f;
	//ロングノーツのスコア増加量
	const uint32_t LONG_NOTE_SCORE_INCREASE_VALUE_ = 100u;
	//開始オフセット
	const float_t START_OFFSET_TIME_ = 1.0f;
	//初期のコンボボーナス倍率
	const float_t INITIAL_COMBO_BONUS_SCALE_ = 1.0f;
	static const uint8_t LANE_SIZE_ = static_cast<uint8_t>(NoteLane::Place::Size);
private:

	//背景
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};

	//オブジェクトプールで管理するための変数
	//通常タップノーツの固定配列
	std::array<std::unique_ptr<NormalTapNote>, NORMAL_NOTE_MAX_SIZE_> normalTapNoteArray_ = {};
	//ロング開始ノーツ
	std::array<std::unique_ptr<HighPassLongNote>, HI_PASS_LONG_NOTE_MAX_SIZE_> highPassLongNoteArray_ = {};
	//ロング(ローパス)開始ノーツ
	std::array<std::unique_ptr<LowPassLongNote>, LOW_PASS_LONG_NOTE_MAX_SIZE_> lowPassLongNoteArray_ = {};
	//ロング(トランスゲート8)開始ノーツ
	std::array<std::unique_ptr<TranceGateEighthLongNote>, TRANS_GATE_EIGHTH_LONG_NOTE_MAX_SIZE_> transGateEighthLongNoteArray_ = {};
	//ロング(トランスゲート16)開始ノーツ
	std::array<std::unique_ptr<TranceGateSixteenthLongNote>, TRANS_GATE_SIXTEENTH_LONG_NOTE_MAX_SIZE_> transGateSixteenthLongNoteArray_ = {};

	//判定線
	std::unique_ptr<JudgementLine> judgementLine_ = nullptr;
	//譜面データ
	std::vector<ExistMusicScoreData> scoreData_ = {};

	//ハイスピ
	float_t hiSpeed_ = 3.0f;

	//再生しているかどうか
	bool isPlay_ = false;
	//停止しているかどうか
	bool isStop_ = true;
	//彩度再生
	bool isResume_ = false;

	//開始時間
	float_t startTime_ = 0.0f;
	//再生時間
	float_t musicTime_ = 0.0f;

	//現在設定中の情報
	size_t currentMusicNumber_ = 0u;
	size_t currentDifficultyNumber_ = 0u;

	//譜面データ
	ExistMusicScoreData existMusicScoreData_ = {};

	std::vector<std::string> difficultyVector_ = {};

	//レーンの状態
	LaneCondition upLaneCondition = {};
	LaneCondition downLaneCondition = {};

	//記録
	NoteJudgement::Record record_ = {};
	float_t comboBonusScale_ = 1.0f;

	//レーンのY座標
	const std::array<float_t, LANE_SIZE_> LANE_POSITION_Y_ = { 10.0f,0.0f };
};