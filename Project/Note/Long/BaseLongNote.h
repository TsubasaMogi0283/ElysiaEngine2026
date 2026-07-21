#pragma once

/**
 * @file BaseLongNote.h
 * @brief ロングノーツ
 * @author 茂木翼
 */

#include <Note/BaseNote.h>

/// <summary>
/// ロングノーツ
/// </summary>
class BaseLongNote : public BaseNote{
public:

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="baseLight">ライト</param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight)override;

public:
	/// <summary>
	/// 終点の座標を設定
	/// </summary>
	/// <param name="positionX">X座標</param>
	inline void SetEndPositionX(const float_t positionX) {
		this->endPositionX_ = positionX;
	}

	/// <summary>
	/// X軸スケールの設定
	/// </summary>
	/// <param name="scaleX">スケールZ</param>
	inline void SetScaleX(const float_t& scaleX) {
		this->worldTransform_.scale.x = scaleX;
	}

	/// <summary>
	/// ホールド状態を設定
	/// </summary>
	/// <param name="isHold">ホールド状態</param>
	inline void SetIsHold(const bool& isHold) {
		this->isHold_ = isHold;
	}

	/// <summary>
	/// ホールド状態を取得
	/// </summary>
	/// <returns>ホールド状態</returns>
	inline bool GetIsHold()const {
		return isHold_;
	}

	/// <summary>
	/// 失敗状態を設定
	/// </summary>
	/// <param name="isMiss">失敗状態</param>
	inline void SetIsMiss(const bool& isMiss) {
		this->isHold_ = isMiss;
	}

	/// <summary>
	/// 失敗状態を取得
	/// </summary>
	/// <returns>失敗状態</returns>
	inline bool GetIsMiss()const {
		return isMiss_;
	}

protected:
	//終点座標
	float_t endPositionX_ = 0.0f;

	//ホールド中
	bool isHold_ = false;
	//失敗時
	bool isMiss_ = false;
};

