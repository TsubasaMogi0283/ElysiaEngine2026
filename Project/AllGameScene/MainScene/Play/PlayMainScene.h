#pragma once

/**
 * @file PlayMainScene.h
 * @brief プレイメインシーンのクラス
 * @author 茂木翼
 */

#include <vector>

#include <MainScene/BaseMainScene.h>
#include <Note/BaseNote.h>

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
	/// <param name="gameManager">ゲーム管理クラス</param>
	/// <param name="mainScene">メインシーン</param>
	void Initialize(Elysia::GameManager* gameManager, MainScene* mainScene)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mainScene">メインシーン</param>
	void Update(MainScene* mainScene)override;

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

	/// <summary>
	/// レーンの位置
	/// </summary>
	enum LanePlace {
		UpLane,
		DownLane,
		LanePlaceSize
	};
private:

	//流れに関するのはX・Y軸しかないのでZは無し
	const float_t JUDGEENT_POSITION_Y_ = -10.0f;
	const std::array<Vector2, LanePlace::LanePlaceSize> JUDGEENT_POSITION_ = {};

	//動き始める時間のオフセット
	const float_t NOTE_MOVE_START_TIME_OFFSET_ = 2.0f;

private:
	
	//ノーツの処理が終了したかどうか
	bool isNoteFlowEnd_ = false;

};

