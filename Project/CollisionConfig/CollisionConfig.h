#pragma once
/**
 * @file CollisionConfig.h
 * @brief コリジョン設定クラス
 * @author 茂木翼
 */

#include <cstdint>
namespace CollisionConfig {
	//未定
	const uint32_t COLLISION_ATTRIBUTE_NONE = 0b0;

	//プレイヤー陣営
	const uint32_t COLLISION_ATTRIBUTE_PLAYER = 0b1;

	//オーディオオブジェクト(レベルエディタ)
	const uint32_t COLLISION_ATTRIBUTE_AUDIO_OBJECT = 0b1 << 8;

	//ステージオブジェクト(レベルエディタ)
	const uint32_t COLLISION_ATTRIBUTE_STAGE_OBJECT = 0b1 << 10;

}