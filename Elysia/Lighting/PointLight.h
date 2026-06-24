#pragma once
/**
 * @file PointLight.h
 * @brief 点光源の構造体
 * @author 茂木翼
 */

#include <Vector4.h>
#include <Vector3.h>
#include "DirectXSetup.h"
#include <BaseLight.h>

/// <summary>
/// 点光源のデータ
/// </summary>
struct PointLightData {
	//ライトの色
	Vector4 color;
	//ライトの位置
	Vector3 position;
	//輝度
	float_t intensity;

	//ライトに届く最大距離
	float_t radius;
	//減衰率
	float_t decay;
	//埋め合わせ
	float_t padding[2];

};

/// <summary>
/// 点光源
/// </summary>
struct PointLight : public BaseLight {
#pragma region メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

#pragma endregion

#pragma region メンバ変数

	//位置
	Vector3 position = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	//ライトに届く最大距離
	float_t radius = 5.0f;
	//減衰率
	float_t decay = 5.0f;

	//書き込みデータ
	PointLightData* pointLightData = nullptr;

#pragma endregion

};