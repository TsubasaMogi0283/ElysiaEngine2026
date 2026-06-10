#pragma once
/**
 * @file VignettePostEffect.h
 * @brief ビネットポストエフェクトのクラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "Vector3.h"
#include <BasePostEffect.h>

/// <summary>
/// ベクトル(4次元)
/// </summary>
struct Vector4;

/// <summary>
/// ビネット
/// </summary>
struct Vignette;


/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class WindowsSetup;

	/// <summary>
	/// DirectXクラス
	/// </summary>
	class DirectXSetup;

	/// <summary>
	/// パイプライン管理クラス
	/// </summary>
	class PipelineManager;

	/// <summary>
	/// RTV管理クラス
	/// </summary>
	class RtvManager;

	/// <summary>
	/// SRV管理クラス
	/// </summary>
	class SrvManager;



	/// <summary>
	/// ビネットポストエフェクト
	/// </summary>
	class VignettePostEffect : public BasePostEffect{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		VignettePostEffect();

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
		/// <param name="vignette">ビネット</param>
		void Draw(const Vignette& vignette);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~VignettePostEffect() = default;
	};
};