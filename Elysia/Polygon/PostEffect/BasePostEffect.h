#pragma once
/**
 * @file BasePostEffect.h
 * @brief ポストエフェクト用の基底クラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {

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
	/// ポストエフェクトの基底クラス
	/// </summary>
	class BasePostEffect {
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize()=0;

		/// <summary>
		/// 描画前処理
		/// </summary>
		virtual void PreDraw() = 0;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~BasePostEffect() = default;

	public:
		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		inline void SetColor(const Vector4& color) {
			this->color_ = color;
		}

	protected:
		//DirectXクラス
		Elysia::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Elysia::PipelineManager* pipelineManager_ = nullptr;
		//RTV管理クラス
		Elysia::RtvManager* rtvManager_ = nullptr;
		//SRV管理クラス
		Elysia::SrvManager* srvManager_ = nullptr;

	protected:
		//横
		uint32_t width_ = 0u;
		//縦
		uint32_t height_ = 0u;

		//SRVハンドル
		uint32_t srvHandle_ = 0;
		//カラー
		Vector4 color_ = { .x = 0.1f,.y = 0.1f,.z = 0.7f,.w = 1.0f };

		//RTV
		//リソース
		ComPtr<ID3D12Resource> rtvResource_ = nullptr;
		//ハンドル
		uint32_t rtvHandle_ = 0;
		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};
	};
}