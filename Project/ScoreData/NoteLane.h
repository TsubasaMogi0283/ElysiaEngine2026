#pragma once
/**
 * @file NoteLane.h
 * @brief レーンのノーツ情報
 * @author 茂木翼
 */

#include <cstdint>

/// <summary>
/// レーンのノーツ情報
/// </summary>
struct NoteLane {
	//上
	uint8_t up = 0;
	//下
	uint8_t down = 0;
};