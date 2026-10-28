#pragma once

/**
 * @file JudgementLine.h
 * @brief 判定線クラス
 * @author 茂木翼
 */
#include <array>
#include <Model.h>
#include <WorldTransform.h>
#include <Material.h>
#include <Sprite.h>
#include <Note/NoteLane.h>
#include <Note/NoteJudgement.h>

/// <summary>
/// 判定線クラス
/// </summary>
class JudgementLine{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	JudgementLine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="normalNoteModelHandle">通常ノーツのモデルハンドル</param>
	/// <param name="judgementTextureHandleArray">判定のテクスチャーハンドル</param>
	void Initialize(const uint32_t& normalNoteModelHandle,const std::array<uint32_t, static_cast<uint8_t>(NoteJudgement::Selection::Size)> judgementTextureHandleArray);

	/// <summary>
	/// 3Dオブジェクトの更新
	/// </summary>
	void UpdateObject3D();

	/// <summary>
	/// スプライトの更新
	/// </summary>
	void UpdateSprite();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~JudgementLine()=default;

public:
	/// <summary>
	/// 成功判定UIを出す設定をする
	/// </summary>
	/// <param name="place"></param>
	/// <param name="selection"></param>
	void SetReleaseUI(const NoteLane::Place& place,const NoteJudgement::Selection& selection);

public:
	/// <summary>
	/// 判定線のX座標を取得
	/// </summary>
	/// <returns>X座標</returns>
	inline float_t GetJudgementPositionX()const {
		return LINE_POSITION_X_;
	}

private:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;

private:
	//判定のX座標
	const float_t LINE_POSITION_X_ = -17.0f;
	
	//判定のUIの数
	static const uint8_t UI_AMOUNT_ = 8u;
	//判定の動く時間
	const float_t UI_MOVE_TIME_ = 0.15f;
	//判定の出る時間
	const float_t APPEAR_TIME_ = UI_MOVE_TIME_ * 2.0f;

	//判定の移動距離
	const float_t UI_MOVE_DISTANCE_ = 100u;
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//UIの初期座標
	const float_t INITIAL_UI_POSITION_X = 150.0f;

private:

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
		NoteLane::Place place = NoteLane::Place::Up;
		//初期の座標
		float_t initialPositionY = {};
		//動いた後の座標
		float_t movedPositionY = {};

		//テクスチャーハンドル
		uint32_t textureHandle = 0u;
		//判定
		NoteJudgement::Selection judgement = NoteJudgement::Selection::Size;
	
	};

private:
	//モデル
	std::unique_ptr<Elysia::Model> lineModel_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform lineWorldTransform_ = {};
	//マテリアル
	Material lineMaterial_ = {};

	//判定のUI
	//成功
	std::array<JudgementUI, UI_AMOUNT_ >judgementUIArray_;
	//ハンドル
	std::array<uint32_t, static_cast<uint8_t>(NoteJudgement::Selection::Size)> judgementTextureHandleArray_ = {};
};