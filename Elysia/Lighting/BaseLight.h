#pragma once

/**
 * @file BaseLight.h
 * @brief ライトの基底構造体
 * @author 茂木翼
 */

#include <Vector4.h>
#include <LightingType.h>
#include <DirectXSetup.h>

/// <summary>
/// ライトの基底構造体
/// </summary>
struct BaseLight {
#pragma region メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

#pragma endregion

#pragma region メンバ変数
	//ライトの種類
	uint8_t lightingType = LightingType::DirectionalLighting;
	//ライトの色
	Vector4 color = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
	//輝度
	float_t intensity = 5.0f;
	//定数バッファ
	ComPtr<ID3D12Resource> resource = nullptr;

#pragma endregion

};