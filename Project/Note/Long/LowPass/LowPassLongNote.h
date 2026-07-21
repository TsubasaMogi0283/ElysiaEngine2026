#pragma once

/*
* @file LowPassLongNote.h
* @brief ローパス用のロングノーツ
* @author 茂木翼
*/

#include <Note/Long/BaseLongNote.h>

/// <summary>
/// ローパス用のロングノーツ
/// </summary>
class LowPassLongNote : public BaseLongNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LowPassLongNote() = default;

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
	~LowPassLongNote()override = default;
};

