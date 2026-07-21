#pragma once

/*
* @file HighPassLongNote.h
* @brief パイパス用のロングノーツ
* @author 茂木翼
*/

#include <Note/Long/BaseLongNote.h>

/// <summary>
/// ハイパス用のロングノーツ
/// </summary>
class HighPassLongNote : public BaseLongNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HighPassLongNote() = default;

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
	~HighPassLongNote()override = default;
};

