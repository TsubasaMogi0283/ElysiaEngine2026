#pragma once

/**
 * @file Gauge.h
 * @brief ゲージのクラス
 * @author 茂木翼
 */

#include <Sprite.h>

/// <summary>
/// ElysiaEngine(前方宣言)
/// </summary>
namespace Elysia {
	/// <summary>
	/// ウィンドウ管理クラス
	/// </summary>
	class WindowsSetup;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;
}

/// <summary>
/// ゲージ
/// </summary>
class Gauge{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Gauge();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mainTextureHandle">メインのテクスチャハンドル</param>
	/// <param name="frameTextureHandle">フレームのテクスチャハンドル</param>
	void Initialize(const uint32_t& mainTextureHandle, const uint32_t& frameTextureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Gauge() = default;

public:

	/// <summary>
	/// ゲージの座標を設定
	/// </summary>
	/// <param name="positionY">Y座標</param>
	inline void SetGaugePositionY(const float_t & positionY) {
		this->back_->SetPosition({ .x = initialPosition_.x, .y = static_cast<int32_t>(positionY) });
		this->main_->SetPosition({.x=initialPosition_.x, .y=static_cast<int32_t>(positionY)});
		this->frame_->SetPosition({ .x = initialPosition_.x, .y = static_cast<int32_t>(positionY) });
	}

	/// <summary>
	/// ゲージのスケールを設定
	/// </summary>
	/// <param name="scale">スケール</param>
	inline void SetGaugeScale(const Vector2<float_t>& scale) {
		this->back_->SetScale(scale);
		this->main_->SetScale(scale);
		this->frame_->SetScale(scale);
	}

	/// <summary>
	/// 初期ゲージ座標を取得
	/// </summary>
	/// <returns>初期ゲージ座標</returns>
	inline Vector2<int32_t> GetInitialPosition()const {
		return initialPosition_;
	}

	/// <summary>
	/// ゲージの通常表示座標を取得
	/// </summary>
	/// <returns>ゲージの通常表示座標</returns>
	inline Vector2<int32_t> GetGaugePosition()const {
		return gaugePosition_;
	}

	/// <summary>
	/// 総ノーツ数を設定
	/// </summary>
	/// <param name="totalNotes">総ノーツ数</param>
	inline void SetTotalNotes(const uint16_t& totalNotes) {
		this->totalNotes_ = totalNotes;
	}

public:
	/// <summary>
	/// ゲージの値を増加させる
	/// </summary>
	/// <param name="result">判定結果</param>
	void SetIncreaseValue(const int32_t& result);


private:
	//ウィンドウ管理クラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;

private:
	//最大
	const uint32_t MAX_VALUE_ = 500u;
	//最小
	const uint32_t MIN_VALUE_ = 0u;
private:
	//バックグラウンドスプライト
	std::unique_ptr<Elysia::Sprite>back_ = nullptr;
	//メイン増減部分のスプライト
	std::unique_ptr<Elysia::Sprite>main_ = nullptr;
	//フレームスプライト
	std::unique_ptr<Elysia::Sprite>frame_ = nullptr;
	//ゲージ値
	uint32_t currentValue_ = 0u;

	//ゲージの座標
	//初期座標
	Vector2<int32_t>initialPosition_ = {};
	//通常座標
	Vector2<int32_t>gaugePosition_ = {};

	//総ノーツ数
	uint16_t totalNotes_ = 0u;

	Vector4<float_t> mainColor_ = {};

	//スケールを設定
	float_t scale_ = static_cast<float_t>(currentValue_) / static_cast<float_t>(MAX_VALUE_);

};

