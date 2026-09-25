#pragma once

/**
 * @file Transition.h
 * @brief トランジションクラス
 * @author 茂木翼
 */

#include <array>

#include <Sprite.h>

/// <summary>
/// ゲーム管理クラス(前方宣言)
/// </summary>
class GameManager;

/// <summary>
/// トランジション
/// </summary>
class Transition{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Transition() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	~Transition() = default;

public:

	/// <summary>
	/// トランジション(開ける)するかどうかを設定
	/// </summary>
	bool SetOpenTransition();

private:

	//トランジション用の画像の分割数
	static const uint8_t DIVIDE_NUMBER_ = 11u;
	//時間変化
	const float_t DELTA_TIME_ = 1.0f/60.0f;
private:
	//トランジションのテクスチャ
	//スプライト
	std::array<std::unique_ptr<Elysia::Sprite>, DIVIDE_NUMBER_> spriteArray_;
	uint32_t textureHandle_ = 0u;

	//横のスケール
	float_t widthScale_ = 1.0f;

	//高さのスケール
	float_t heightScale_ = 0.0f;
	

	//開いているかどうか
	bool isOpened_ = false;
	//閉まっているかどうか
	bool isClosed_ = true;

	//トランジションの進行度
	float_t openT_ = 0.0f;
};

