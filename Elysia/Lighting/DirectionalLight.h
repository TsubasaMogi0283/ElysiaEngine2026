#pragma once
/**
 * @file DirectionalLight.h
 * @brief 平行光源の構造体
 * @author 茂木翼
 */

#include "Vector4.h" 
#include "Vector3.h"
#include "DirectXSetup.h"
#include <BaseLight.h>

/// <summary>
/// 平行光源データ
/// </summary>
struct DirectionalLightData {
	//ライトの色
	Vector4 color;
	//ライトの向き
	Vector3 direction;
	//輝度
	float_t intensity;
};


/// <summary>
/// 平行光源
/// </summary>
struct DirectionalLight : public BaseLight{
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

	//ライトの向き
	Vector3 direction = {.x = 0.0f,.y = -1.0f,.z = 0.0f};

	//書き込みデータ
	DirectionalLightData* directionalLightData_ = nullptr;

#pragma endregion

};
