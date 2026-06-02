#pragma once
/**
 * @file SepiaScale.h
 * @brief セピアのクラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "BasePostEffect.h"

 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {

	/// <summary>
	/// セピア
	/// </summary>
	class SepiaScale : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		SepiaScale();

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
		~SepiaScale() = default;

	};
};