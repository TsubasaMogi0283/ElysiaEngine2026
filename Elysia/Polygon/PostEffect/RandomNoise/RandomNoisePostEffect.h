#pragma once
#pragma once

/**
 * @file RandomNoisePostEffect.h
 * @brief ランダムノイズポストエフェクトのクラス
 * @author 茂木翼
 */

#include <time.h>
#include <random>

#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "RandomNoise.h"
#include "BasePostEffect.h"


/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// ランダムノイズポストエフェクト
	/// </summary>
	class RandomNoisePostEffect : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		RandomNoisePostEffect();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()override;

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw()override;

		/// <summary>
		/// 描画
		/// </summary>
		void Draw()override;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~RandomNoisePostEffect() = default;
	public:
		/// <summary>
		/// 強さの設定
		/// </summary>
		/// <param name="strength"></param>
		inline void SetStrength(const float_t& strength) {
			this->randomNoise_.strength = strength;
		}

		/// <summary>
		/// 値の設定
		/// </summary>
		/// <param name="value"></param>
		inline void SetValue(const float_t& value) {
			this->randomNoise_.value = value;
		}

		/// <summary>
		/// テクスチャを使用するかの設定
		/// </summary>
		/// <param name="isUseTexture"></param>
		inline void SetIsUseTexture(const bool& isUseTexture) {
			this->randomNoise_.isUseTexture = isUseTexture;
		}

	private:
		//ランダムノイズ
		RandomNoise randomNoise_ = {};
	};
};
