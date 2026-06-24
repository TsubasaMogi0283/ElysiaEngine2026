#pragma once

/**
 * @file LongNote.h
 * @brief ロングノーツ
 * @author 茂木翼
 */

#include <Note/BaseNote.h>

/// <summary>
/// ロングノーツ
/// </summary>
class LongNote : public BaseNote{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LongNote();

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
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight) override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LongNote() = default;

private:


};

