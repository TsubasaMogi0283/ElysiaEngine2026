#pragma once

/**
 * @file Listener.h
 * @brief リスナー構造体
 * @author 茂木翼
 */

#include "Vector3.h"

/// <summary>
/// リスナー
/// </summary>
struct Listener {
	//位置
	Vector3<float_t> position;
	//動き
	Vector3<float_t> move;
};