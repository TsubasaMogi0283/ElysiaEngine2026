#pragma once
/**
 * @file BackTexture.h
 * @brief ポストエフェクトのクラス
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
	/// ポストエフェクト(通常背景用)
	/// </summary>
	class BackTexture : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		BackTexture();

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
		~BackTexture() = default;

	private:
		//リソース
		ComPtr<ID3D12Resource> effectResource_ = nullptr;

		int32_t* effectTypeData_ = nullptr;

	};
}