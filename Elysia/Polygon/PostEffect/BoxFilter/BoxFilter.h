#pragma once
/**
 * @file BoxFilter.h
 * @brief ボックスフィルタのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "BasePostEffect.h"

 /// <summary>
 /// ボックスフィルタの種類
 /// </summary>
enum BoxFilterSelection {
	BoxFilter3x3,
	BoxFilter5x5,

};

/// <summary>
/// 平滑化に関するデータ
/// </summary>
struct BoxFilterType {

	//Smoothing(平滑化)
	//輪郭などのくっきりしたところをぼかして滑らかな雰囲気を出すよ
	int32_t type;

};

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// ボックスフィルタ
	/// </summary>
	class BoxFilter : public BasePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		BoxFilter();

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
		~BoxFilter() = default;

	private:
		//リソース
		ComPtr<ID3D12Resource> boxFilterTypeResource_ = nullptr;
		//データ
		BoxFilterType* boxFilterTypeData_ = nullptr;
		//型
		uint32_t boxFilterType_ = BoxFilter3x3;
	};
}