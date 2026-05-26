#pragma once
/**
 * @file GrayScale.h
 * @brief グレースケール
 * @author 茂木翼
 */


#include "DirectXSetup.h"



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
	/// グレースケール
	/// </summary>
	class GrayScale {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GrayScale();

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
		void Draw();

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GrayScale() = default;

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
		uint32_t rtvHandle_ = 0;

		//SRVハンドル
		uint32_t srvHandle_ = 0;

	};

}