#pragma once

/**
 * @file Dissolve.h
 * @brief ディゾルブのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "Dissolve.h"
#include "BasePostEffect.h"


/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// ディゾルブ(ポストエフェクト)
	/// </summary>
	class DissolvePostEffect : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DissolvePostEffect();

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
		~DissolvePostEffect() = default;
	
	public:
		/// <summary>
		/// ディゾルブのデータを設定
		/// </summary>
		/// <param name="dissolve"></param>
		inline void SetDissolveData(const Dissolve& dissolve) {
			this->dissolve_ = dissolve;
		}

	private:
		//ディゾルブ
		Dissolve dissolve_ = {};

	};
}