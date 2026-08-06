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
	void Initilaize(const uint32_t& blackTextureHandle, const std::vector<uint32_t>& countNumberTextureHandles);

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
	/// カウントダウン状態の取得
	/// </summary>
	/// <returns>カウントダウン状態</returns>
	inline bool GetIsCountDown()const {
		return isCountDown_;
	}

	/// <summary>
	/// カウントダウン状態の設定
	/// </summary>
	/// <param name="isCountDown">カウントダウン状態</param>
	inline void SetIsCountDown(const bool& isCountDown) {
		this->isCountDown_ = isCountDown;
	}

	/// <summary>
	/// 終了状態の取得
	/// </summary>
	/// <returns>終了状態</returns>
	inline bool GetIsEnd()const {
		return isEnd_;
	}

	/// <summary>
	/// 終了状態の設定
	/// </summary>
	/// <param name="isEnd">終了状態</param>
	inline void SetIsEnd(const bool& isEnd) {
		this->isEnd_ = isEnd;
	}

private:
	//ポーズ時の透明度
	const float_t PAUSE_TRANSPARENCY_ = 0.8f;
	//ポーズ時間
	const float_t COUNT_DOWN_TIME_ = 3.0f;
	//透明化開始時間
	const float_t TRANSPARENCY_START_TIME_ = 1.0f;
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//カウントダウンのスケールサイズ(オフセット)
	const float_t COUNT_DOWN_OFFSET_SCALE_SIZE_ = 0.3f;

private:

	/// <summary>
	/// テクスチャや数字の情報
	/// </summary>
	struct NumberInformation {
		//ハンドル
		uint32_t textureHandle = 0u;
		//数字
		uint8_t number = 0u;
	};

private:
	//カウントダウン用のスプライト
	std::vector<NumberInformation>numberInformationVector_ = {};
	//数字
	std::unique_ptr<Elysia::Sprite>numberSprite_ = nullptr;
	//アンカーポイント
	Vector2 anchorPoint_ = {0.5f, 0.5f};
	//回転
	float_t numberSpriteRotate_ = 0.0f;

	//ポーズ中少し暗くするためのスプライト
	std::unique_ptr<Elysia::Sprite> backSprite_ = nullptr;
	float_t transparencyT = 0.0f;
	
	//時間
	float_t timer_ = 3.0f;
	//表示設定
	bool isDisplay_ = true;
	//現在の数字テクスチャハンドル
	uint32_t currentNumberTextureHandle_ = 0u;
	
	//ポーズから再開したときの時間
	float_t restartTimer_ = COUNT_DOWN_TIME_;

	//再開したかどうか
	bool isCountDown_ = false;
	//終了
	bool isEnd_ = false;
};