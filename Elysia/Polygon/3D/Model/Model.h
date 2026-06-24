#pragma once
/**
 * @file Model.h
 * @brief モデル
 * @author 茂木翼
 */

#include "DirectXSetup.h"

#include "Matrix4x4.h"
#include "Vector4.h"
#include "TransformationMatrix.h"
#include "VertexData.h"
#include "LightingType.h"
#include "ModelData.h"

#pragma region 前方宣言



 /// <summary>
 /// ワールドトランスフォーム
 /// </summary>
struct WorldTransform;

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// GPUに送るためのカメラ
/// </summary>
struct CameraForGPU;

/// <summary>
/// マテリアル
/// </summary>
struct Material;

/// <summary>
/// 基底ライト
/// </summary>
struct BaseLight;
#pragma endregion


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
	/// SRV管理クラス
	/// </summary>
	class SrvManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;


	/// <summary>
	/// モデル
	/// </summary>
	class Model {
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Model();

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="modelHandle">モデルハンドル</param>
		/// <returns></returns>
		static std::unique_ptr<Model> Create(const uint32_t& modelHandle);


		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="worldTransform"></param>
		/// <param name="camera"></param>
		/// <param name="material"></param>
		void Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="camera">カメラ</param>
		/// <param name="baseLight">ライト</param>
		void Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material, const BaseLight& baseLight);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Model() = default;


	public:
		/// <summary>
		/// 環境マップテクスチャの設定
		/// </summary>
		/// <param name="textureHandle"></param>
		inline void SetEnvironmentTexture(const uint32_t& textureHandle) {
			this->environmentTextureHandle_ = textureHandle;
		}

		/// <summary>
		/// 非表示設定
		/// </summary>
		/// <param name="isInvisible"></param>
		inline void SetInvisible(const bool& isInvisible) {
			this->isInvisible_ = isInvisible;
		}



	private:
		//DirectXクラス
		Elysia::DirectXSetup* directXSetup_ = nullptr;
		//テクスチャ管理クラス
		Elysia::TextureManager* textureManager_ = nullptr;
		//モデル管理クラス
		Elysia::ModelManager* modelManager_ = nullptr;
		//パイプライン管理クラス
		Elysia::PipelineManager* pipelineManager_ = nullptr;
		//SRV管理クラス
		Elysia::SrvManager* srvManager_ = nullptr;

	private:
		//頂点リソース
		ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

		//インデックスリソース
		ComPtr<ID3D12Resource> indexResource_ = nullptr;
		//インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		//カメラリソース
		ComPtr<ID3D12Resource> cameraResource_ = nullptr;
		//PixelShaderにカメラの座標を送る為の変数
		CameraForGPU* cameraForGPU_ = {};

		//テクスチャハンドル
		uint32_t textureHandle_ = 0u;
		//環境マップ
		uint32_t environmentTextureHandle_ = 0;

		//非表示設定
		bool isInvisible_ = false;

		//モデルハンドル
		ModelData modelData_ = {};

	};
}