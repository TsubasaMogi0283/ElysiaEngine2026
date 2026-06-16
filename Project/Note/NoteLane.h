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
	//左
	uint8_t left = 0;
	//左中
	int leftMiddle = 0;
	//右中
	int rightMiddle = 0;
	//右
	int right = 0;
};