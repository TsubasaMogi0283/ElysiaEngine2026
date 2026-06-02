#pragma once
/**
 * @file GrayScale.h
 * @brief グレースケール
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "BasePostEffect.h"

 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {

	/// <summary>
	/// グレースケール
	/// </summary>
	class GrayScale : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GrayScale();

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
		~GrayScale() = default;
	};
}