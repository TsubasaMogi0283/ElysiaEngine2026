#pragma once
/**
 * @file SpotLight.h
 * @brief スポットライトのクラス
 * @author 茂木翼
 */

#include "Vector4.h"
#include "Vector3.h"
#include "DirectXSetup.h"
#include <BaseLight.h>

/// <summary>
/// スポットライトのデータ
/// </summary>
struct SpotLightData {
	//ライトの色
	Vector4 color = {};
	//ライトの位置
	Vector3 position = {};
	//輝度
	float_t intensity = 0.0f;
	//スポットライトの方向
	Vector3 direction = {};
	//ライトの届く最大距離
	float_t distance = 0.0f;
	//減衰率
	float_t decay = 0.0f;
	//FallowOffを制御する
	float_t cosFallowOffStart = 0.0f;
	//スポットライトの余弦
	float_t cosAngle = 0.0f;
	//当たっていない所をこれで明るくする
	float_t aroundOffset = 0.0f;
	//埋め合わせ
	float_t padding[2] = {};

};

 /// <summary>
 /// スポットライト
 /// </summary>
struct SpotLight : public BaseLight {
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

	//ライトの位置
	Vector3 position = {};
	//方向
	Vector3 direction = {};
	//届く最大距離
	float_t distance = 0.0f;
	//減衰率
	float_t decay = 0.0f;
	//FallowOffを制御する
	float_t cosFallowOffStart = 0.0f;
	//スポットライトの余弦
	float_t cosAngle = 0.0f;
	//当たっていない所をこれで明るくする
	float_t aroundOffset = 0.0f;

	//書き込みデータ
	SpotLightData* data_ = nullptr;

#pragma endregion

};