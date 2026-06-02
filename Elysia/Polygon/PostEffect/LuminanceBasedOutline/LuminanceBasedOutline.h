#pragma once

/**
 * @file LuminanceBasedOutline.h
 * @brief アウトライン(輝度)の管理クラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "../Polygon/PostEffect/BasePostEffect.h"

 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {

	/// <summary>
	/// アウトライン(輝度)
	/// </summary>
	class LuminanceBasedOutline : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		LuminanceBasedOutline();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()override;

		/// <summary>
		/// 描画前の処理
		/// </summary>
		void PreDraw()override;

		/// <summary>
		/// 描画
		/// </summary>
		void Draw()override;


		/// <summary>
		/// デストラクタ
		/// </summary>
		~LuminanceBasedOutline() = default;
	};
}