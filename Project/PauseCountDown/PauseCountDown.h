#pragma once

/**
 * @file PauseAssets.h
 * @brief ポーズクラス
 * @author 茂木翼
 */

#include <array>
#include <memory>
#include <Sprite.h>

 /// <summary>
 /// ポーズセット
 /// </summary>
class PauseAssets {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseAssets() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initilaize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PauseAssets() = default;

public:
	/// <summary>
	/// 時間の設定
	/// </summary>
	/// <param name="timer">カウントダウンの時間</param>
	inline void SetTimer(const float_t& timer){
		this->timer_ = timer;
	}

	/// <summary>
	/// 表示の設定
	/// </summary>
	/// <param name="isDisplay"></param>
	inline void SetIsDisplay(const bool& isDisplay) {
		this->isDisplay_ = isDisplay;
	}

private:
	//数字の量
	static const uint8_t COUNT_DOWN_NUMBER_ = 3u;

private:
	//カウントダウン用のスプライト
	std::array<std::unique_ptr<Elysia::Sprite>, COUNT_DOWN_NUMBER_>spriteArray_ = {};

	//時間
	float_t timer_ = 0.0f;
	//表示設定
	bool isDisplay_ = true;

};

