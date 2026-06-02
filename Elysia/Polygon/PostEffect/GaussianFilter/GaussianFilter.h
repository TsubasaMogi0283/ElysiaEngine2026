#pragma once
/**
 * @file GaussianFilter.h
 * @brief ガウシアンフィルタのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "BasePostEffect.h"

 /// <summary>
 /// 種類
 /// </summary>
enum GaussianFilterSelection {
	//GaussianFilter
	//BoxFilterよりこっちの方良い感じらしい
	GaussianFilter3x3,
	GaussianFilter5x5,
};

/// <summary>
/// データ
/// </summary>
struct GaussianFilterData {
	//種類
	int32_t type;

	//標準偏差
	//StandardDivision。一般的にσ(シグマ)が使われる
	float_t sigma;

};

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// ガウシアンフィルタ
	/// </summary>
	class GaussianFilter : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GaussianFilter();

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
		~GaussianFilter() = default;
	public:
		/// <summary>
		/// タイプの設定
		/// </summary>
		/// <param name="type"></param>
		inline void SetType(int32_t type) {
			this->boxFilterType_ = type;
		}

		/// <summary>
		/// シグマの設定
		/// </summary>
		/// <param name="sigma"></param>
		inline void SetSigma(float_t sigma) {
			this->sigma_ = sigma;
		}

	private:
		//リソース
		ComPtr<ID3D12Resource> gaussianFilterTypeResource_ = nullptr;
		GaussianFilterData* gaussianFilterTypeData_ = nullptr;
		int32_t boxFilterType_ = GaussianFilter3x3;
		float_t sigma_ = 0.0f;

	};
}