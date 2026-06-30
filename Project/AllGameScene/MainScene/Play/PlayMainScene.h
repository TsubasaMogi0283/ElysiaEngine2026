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
	~PlayMainScene()override = default;

private:

	//流れに関するのはX・Y軸しかないのでZは無し
	const float_t JUDGEENT_POSITION_Y_ = -10.0f;
	const std::array<Vector2, NoteLane::Place::Size> JUDGEENT_POSITION_ = {};

	//動き始める時間のオフセット
	const float_t NOTE_MOVE_START_TIME_OFFSET_ = 2.0f;

private:
	//楽曲情報
	MusicInformation musicInformation_ = {};
	//譜面情報
	MusicScoreData musicScoreData_ = {};

	//演奏中
	bool isPlay_ = true;
	//ポーズ中
	bool isPause_ = false;
	//再生時間
	float_t musicTime_ = 0.0f;
};