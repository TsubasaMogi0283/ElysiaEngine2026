#pragma once

/**
 * @file BaseNote.h
 * @brief 通常タップノーツ
 * @author 茂木翼
 */

#include <Note/BaseNote.h>

/// <summary>
/// 通常タップノーツ
/// </summary>
class NormalTapNote : public BaseNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	NormalTapNote();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	void Initialize(const uint32_t modelHandle)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void DrawObject3D(const Camera& camera) override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalTapNote() = default;

private:


};

