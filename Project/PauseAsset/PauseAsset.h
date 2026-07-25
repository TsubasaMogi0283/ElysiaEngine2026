#pragma once

/**
 * @file PauseAsset.h
 * @brief ポーズのアセットクラス
 * @author 茂木翼
 */

#include <array>
#include <vector>
#include <memory>
#include <Sprite.h>

 /// <summary>
 /// ポーズのアセット
 /// </summary>
class PauseAsset {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseAsset() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="blackTextureHandle">黒背景のテクスチャーハンドル</param>
	/// <param name="countNumberTextureHandles">カウントダウン用の数字のテクスチャーハンドル</param>
	void Initilaize(const uint32_t& blackTextureHandle,const std::vector<uint32_t>& countNumberTextureHandles);

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
	~PauseAsset() = default;

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
	//ポーズ時の透明度
	const float_t PAUSE_TRANSPARENCY_ = 0.8f;

private:
	//カウントダウン用のスプライト
	std::vector<std::unique_ptr<Elysia::Sprite>>countSpriteVector_ = {};
	//ポーズ中少し暗くするためのスプライト
	std::unique_ptr<Elysia::Sprite> backSprite_ = nullptr;
	//時間
	float_t timer_ = 0.0f;
	//表示設定
	bool isDisplay_ = true;


};