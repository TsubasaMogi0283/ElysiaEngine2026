#pragma once

/**
 * @file JudgementLine.h
 * @brief 判定線クラス
 * @author 茂木翼
 */

#include <Model.h>
#include <WorldTransform.h>
#include <Material.h>

/// <summary>
/// 判定線クラス
/// </summary>
class JudgementLine{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	JudgementLine() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="normalNoteModelHandle">通常ノーツのモデルハンドル</param>
	void Initialize(uint32_t normalNoteModelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void Draw(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~JudgementLine()=default;

public:
	/// <summary>
	/// 判定線のX座標を取得
	/// </summary>
	/// <returns>X座標</returns>
	inline float_t GetJudgementPositionX()const {
		return JUDGEMENT_POSITION_X_;
	}

private:
	//判定のX座標
	const float_t JUDGEMENT_POSITION_X_ = -17.0f;


private:
	//モデル
	std::unique_ptr<Elysia::Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
};