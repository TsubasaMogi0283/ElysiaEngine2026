#pragma once

/*
* @file TransGateEighthLongNote.h
* @brief トランスゲート8用のロングノーツ
* @author 茂木翼
*/

#include <Note/Long/BaseLongNote.h>

/// <summary>
/// トランスゲート8用のロングノーツ
/// </summary>
class TransGateEighthLongNote : public BaseLongNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TransGateEighthLongNote() = default;

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
	~TransGateEighthLongNote()override = default;
};

