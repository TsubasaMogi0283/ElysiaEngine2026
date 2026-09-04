#pragma once

/**
 * @file Transition.h
 * @brief トランジションクラス
 * @author 茂木翼
 */

#include <array>

#include <Sprite.h>


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

	inline void SetTransition(const bool& isTransition) {
		this->isTransition_ = isTransition;
	}
private:
	//トランジション用の画像の分割数
	static const uint8_t DIVIDE_NUMBER_ = 10u;

private:
	//トランジションのテクスチャ
	//スプライト
	std::array<std::unique_ptr<Elysia::Sprite>, DIVIDE_NUMBER_> spriteArray_;
	uint32_t textureHandle_ = 0u;
	float_t heightScale_ = 0.0f;
	
	//トランジションをするかどうか
	bool isTransition_ = false;

};

