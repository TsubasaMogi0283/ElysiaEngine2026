#pragma once

/**
 * @file Animation.h
 * @brief アニメーションの関するクラス
 * @author 茂木翼
 */

#include <map>
#include <string>

#include "NodeAnimation.h"
#include "Joint.h"
#include "Skeleton.h"

/// <summary>
/// アニメーション
/// </summary>
struct Animation {
	//アニメーション全体の尺
	float_t duration=0.0f;
	//NodeAnimationの集合
	//Node名でひけるようにする
	std::map<std::string, NodeAnimation>nodeAnimations = {};
};


