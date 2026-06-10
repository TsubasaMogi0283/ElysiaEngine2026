#pragma once
/**
 * @file PipelineManager.h
 * @brief PSO(パイプライン)の管理クラス
 * @author 茂木翼
 */


#include <d3d12.h>
#include <cassert>

#include "DirectXSetup.h"

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// パイプライン管理クラス
	/// </summary>
	class PipelineManager final {
	private:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		PipelineManager()=default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~PipelineManager() = default;

	public:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static PipelineManager* GetInstance();

		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="pipelineManager"></param>
		PipelineManager(const PipelineManager& pipelineManager) = delete;

		/// <summary>
		/// 代入演算子を無効にする
		/// </summary>
		/// <param name="pipelineManager"></param>
		/// <returns></returns>
		PipelineManager& operator=(const PipelineManager& pipelineManager) = delete;

#pragma region アクセッサ

		//コマンドに積むためのGetter(Line)
		ComPtr<ID3D12RootSignature> GetLineRootSignature() const {
			return linePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetLineGraphicsPipelineState() const {
			return linePSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(Sprite)
		ComPtr<ID3D12RootSignature> GetSpriteRootSignature() const {
			return spritePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() const {
			return spritePSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(Model)
		ComPtr<ID3D12RootSignature> GetModelRootSignature() const {
			return modelPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetModelGraphicsPipelineState() const{
			return modelPSO_.graphicsPipelineState_;
		}


		//コマンドに積む用のGetter(Skinning)
		ComPtr<ID3D12RootSignature> GetAnimationModelRootSignature() const {
			return animationModelPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetAnimationModelGraphicsPipelineState() const {
			return animationModelPSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(Particle3D)
		ComPtr<ID3D12RootSignature> GetParticle3DRootSignature() const{
			return particle3DPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetParticle3DGraphicsPipelineState() const {
			return particle3DPSO_.graphicsPipelineState_;
		}

		//コマンドに積む用のGetter(SkyBox)
		ComPtr<ID3D12RootSignature> GetSkyBoxRootSignature() const {
			return skyBoxPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetSkyBoxGraphicsPipelineState() const {
			return skyBoxPSO_.graphicsPipelineState_;
		}




		//コマンドに積むためのGetter(CopyImage)
		ComPtr<ID3D12RootSignature> GetFullScreenRootSignature() const {
			return fullScreenPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetFullScreenGraphicsPipelineState() const {
			return fullScreenPSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(GrayScale)
		ComPtr<ID3D12RootSignature> GetGrayScaleRootSignature() const {
			return grayScalePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetGrayScaleGraphicsPipelineState() const {
			return grayScalePSO_.graphicsPipelineState_;
		}
		//コマンドに積むためのGetter(SepiaScale)
		ComPtr<ID3D12RootSignature> GetSepiaScaleRootSignature() const {
			return sepiaScalePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetSepiaScaleGraphicsPipelineState() const {
			return sepiaScalePSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(Vignette)
		ComPtr<ID3D12RootSignature> GetVignetteRootSignature() const {
			return vignettePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetVignetteGraphicsPipelineState() const {
			return vignettePSO_.graphicsPipelineState_;
		}
		//コマンドに積むためのGetter(BoxFilter)
		ComPtr<ID3D12RootSignature> GetBoxFilterRootSignature() const {
			return boxFilterPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetBoxFilterGraphicsPipelineState() const {
			return boxFilterPSO_.graphicsPipelineState_;
		}
		//コマンドに積むためのGetter(GaussianFilter)
		ComPtr<ID3D12RootSignature> GetGaussianFilterRootSignature() const {
			return gaussianFilterPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetGaussianFilterGraphicsPipelineState() const {
			return gaussianFilterPSO_.graphicsPipelineState_;
		}


		//コマンドに積むためのGetter(LuminanceBasedOutline)
		ComPtr<ID3D12RootSignature> GetLuminanceBasedOutlineRootSignature() const{
			return luminanceBasedOutlinePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetLuminanceBasedOutlineGraphicsPipelineState() const	{
			return luminanceBasedOutlinePSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(DepthBasedOutline)
		ComPtr<ID3D12RootSignature> GetDepthBasedOutlineRootSignature() const{
			return depthBasedOutlinePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetDepthBasedOutlineGraphicsPipelineState() const {
			return depthBasedOutlinePSO_.graphicsPipelineState_;
		}
		//コマンドに積むためのGetter(RadialBlur)
		ComPtr<ID3D12RootSignature> GetRadialBlurRootSignature() const {
			return radialBlurPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetRadialBlurGraphicsPipelineState() const {
			return radialBlurPSO_.graphicsPipelineState_;
		}

		//コマンドに積むためのGetter(Dissolve)
		ComPtr<ID3D12RootSignature> GetDissolveRootSignature() const {
			return dissolvePSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetDissolveGraphicsPipelineState() const {
			return dissolvePSO_.graphicsPipelineState_;
		}
		//コマンドに積むためのGetter(RandomEffect)
		ComPtr<ID3D12RootSignature> GetRandomEffectRootSignature() const {
			return randomEffectPSO_.rootSignature_;
		}
		ComPtr<ID3D12PipelineState> GetRandomEffectGraphicsPipelineState() const {
			return randomEffectPSO_.graphicsPipelineState_;
		}


		/// <summary>
		/// スプライトのブレンドモードの設定
		/// </summary>
		/// <param name="blendMode"></param>
		void SetSpriteBlendMode(uint32_t blendMode) {
			selectSpriteBlendMode_ = blendMode;
		}
		/// <summary>
		/// モデルのブレンドモードの設定
		/// </summary>
		/// <param name="blendMode"></param>
		void SetModelBlendMode(uint32_t blendMode) {
			selectModelBlendMode_ = blendMode;
		}




#pragma endregion



	private:
		/// <summary>
		/// PSOの情報
		/// </summary>
		struct PSOInformation {
			ComPtr<ID3DBlob> signatureBlob_ = nullptr;
			ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
			ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
			ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
			ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

		};

	private:
		/// <summary>
		/// PSOの生成
		/// </summary>
		/// <param name="psoInformation"></param>
		/// <param name="inputLayoutDesc"></param>
		/// <param name="blendDesc"></param>
		/// <param name="rasterizerDesc"></param>
		static void GeneratePSO(PSOInformation& psoInformation,const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc,const D3D12_BLEND_DESC& blendDesc,const D3D12_RASTERIZER_DESC& rasterizerDesc);

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

	private:

		/// <summary>
		/// ライン用のPSOを生成
		/// </summary>
		static void GeneratedLinePSO();

		/// <summary>
		/// スプライト用のPSOを生成
		/// </summary>
		static void GenerateSpritePSO();

		/// <summary>
		/// モデル用のPSOを生成
		/// </summary>
		static void GenerateModelPSO();

		/// <summary>
		/// SkinningModel用のPSOを生成
		/// </summary>
		static void GenerateAnimationModelPSO();

		/// <summary>
		/// 3Dパーティクル用のPSOを生成
		/// </summary>
		static void GenerateParticle3DPSO();

		/// <summary>
		/// CopyImage用のPSOを生成
		/// </summary>
		static void GenerateFullScreenPSO();

		/// <summary>
		/// GrayScale用のPSOを生成
		/// </summary>
		static void GenerateGrayScalePSO();

		/// <summary>
		/// SepiaScale用のPSOを生成
		/// </summary>
		static void GenerateSepiaScalePSO();

		/// <summary>
		/// Vignette用のPSOを生成
		/// </summary>
		static void GenerateVignettePSO();

		/// <summary>
		/// BoxFilter用のPSOを生成
		/// </summary>
		static void GenerateBoxFilterPSO();

		/// <summary>
		/// GaussianFilter用のPSOを生成
		/// </summary>
		static void GenerateGaussianFilterPSO();

		/// <summary>
		/// OutLine用のPSOを生成
		/// </summary>
		static void GenerateLuminanceBasedOutlinePSO();

		/// <summary>
		/// DepthBasedOutline用
		/// </summary>
		static void GenerateDepthBasedOutlinePSO();

		/// <summary>
		/// RadialBlur用のPSOを生成
		/// </summary>
		static void GenerateRadialBlurPSO();

		/// <summary>
		/// Dissolve用のPSOを生成
		/// </summary>
		static void GenerateDissolvePSO();

		/// <summary>
		/// RandomEffect用のPSOを生成
		/// </summary>
		static void GenerateRandomEffectPSO();


		/// <summary>
		/// SkyBoxのPSOの生成
		/// </summary>
		static void GenerateSkyBoxPSO();

	private:

		//ライン用
		PSOInformation linePSO_ = {};
		//スプライト用
		PSOInformation spritePSO_ = {};
		//モデル用の変数
		PSOInformation modelPSO_ = {};
		//モデル用の変数
		PSOInformation particle3DPSO_ = {};
		//CopyImage用
		PSOInformation fullScreenPSO_ = {};
		//GrayScale用
		PSOInformation grayScalePSO_ = {};
		//SepiaScale用
		PSOInformation sepiaScalePSO_ = {};
		//Vignette用
		PSOInformation vignettePSO_ = {};
		//BoxFilter用
		PSOInformation boxFilterPSO_ = {};
		//GaussianFilter用
		PSOInformation gaussianFilterPSO_ = {};
		//LuminanceBasedOutline用
		PSOInformation luminanceBasedOutlinePSO_ = {};
		//DepthBasedOutline用
		PSOInformation depthBasedOutlinePSO_ = {};
		//RadialBlur用
		PSOInformation radialBlurPSO_ = {};
		//Dissolve用
		PSOInformation dissolvePSO_ = {};
		//RandomEffect用
		PSOInformation randomEffectPSO_ = {};

		//アニメーションモデル用の変数
		PSOInformation animationModelPSO_ = {};
		//スカイボックス用の変数
		PSOInformation skyBoxPSO_ = {};

		//スプライト用
		int32_t selectSpriteBlendMode_ = 1;
		//モデル用の
		int32_t selectModelBlendMode_ = 1;
		//モデル用の
		int32_t selectAnimationModelBlendMode_ = 1;
	};
};