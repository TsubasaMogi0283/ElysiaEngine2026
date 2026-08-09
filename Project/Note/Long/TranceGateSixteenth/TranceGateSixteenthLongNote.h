#pragma once

/*
* @file TranceGateSixteenthLongNote.h
* @brief トランスゲート16用のロングノーツ
* @author 茂木翼
*/

#include <Note/Long/BaseLongNote.h>

/// <summary>
/// トランスゲート16用のロングノーツ
/// </summary>
class TranceGateSixteenthLongNote : public BaseLongNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TranceGateSixteenthLongNote() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	void Initialize(const uint32_t& modelHandle)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TranceGateSixteenthLongNote()override = default;
};

