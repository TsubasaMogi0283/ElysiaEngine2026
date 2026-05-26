#pragma once

/**
 * @file RandomNoisePostEffect.h
 * @brief ランダムノイズポストエフェクトのクラス
 * @author 茂木翼
 */

#include <time.h>
#include <random>

#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

/// <summary>
/// ランダムノイズ
/// </summary>
struct RandomNoise;

 /// <summary>
 /// KamabokoEngine
 /// </summary>
namespace Kamaboko {
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
	/// ランダムノイズポストエフェクト
	/// </summary>
	class RandomNoisePostEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		RandomNoisePostEffect();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw();

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="randomNoise">ランダムノイズ</param>
		void Draw(const RandomNoise& randomNoise);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~RandomNoisePostEffect() = default;

	private:
		//Windowクラス
		Kamaboko::WindowsSetup* windowSetup_ = nullptr;
		//DirectXクラス
		Kamaboko::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Kamaboko::PipelineManager* pipelineManager_ = nullptr;
		//RTV管理クラス
		Kamaboko::RtvManager* rtvManager_ = nullptr;
		//SRV管理クラス
		Kamaboko::SrvManager* srvManager_ = nullptr;

	private:
		
		//RTV
		//リソース
		ComPtr<ID3D12Resource> rtvResource_ = nullptr;
		//ハンドル
		uint32_t rtvHandle_ = 0u;

		//SRVハンドル
		uint32_t srvHandle_ = 0u;

		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};


	};
};
