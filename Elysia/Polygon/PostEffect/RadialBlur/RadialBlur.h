#pragma once
/**
 * @file RadialBlur.h
 * @brief ラジアルブラーのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "BasePostEffect.h"

 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {
	/// <summary>
	/// ラジアルブラー
	/// </summary>
	class RadialBlur : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		RadialBlur();

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
		~RadialBlur() = default;
	};
};