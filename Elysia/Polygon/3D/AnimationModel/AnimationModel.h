#pragma once

/**
 * @file AnimationModel.h
 * @brief アニメーションモデル
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>
#include "ModelData.h"
#include "LightingType.h"
#include "Material.h"
#include "SkinCluster.h"

 /// <summary>
 /// ワールドトランスフォーム
 /// </summary>
struct WorldTransform;

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// GPUに送るカメラ
/// </summary>
struct CameraForGPU;

/// <summary>
/// スキンクラスター
/// </summary>
struct SkinCluster;

/// <summary>
/// 平行光源
/// </summary>
struct DirectionalLight;

/// <summary>
/// 点光源
/// </summary>
struct PointLight;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

/// <summary>
/// マテリアル
/// </summary>
struct Material;


/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// DirectXのクラス
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
	/// アニメーション管理クラス
	/// </summary>
	class AnimationManager;

	/// <summary>
	/// アニメーションモデル
	/// </summary>
	class AnimationModel {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		AnimationModel();

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="modelHandle">モデルハンドル</param>
		/// <param name="animationHandle">アニメーションハンドル</param>
		/// <returns></returns>
		static std::unique_ptr<AnimationModel> Create(const uint32_t& modelHandle, const uint32_t& animationHandle);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="animationTime">アニメーションの時間</param>
		void Update(const float& animationTime);

		/// <summary>
		/// 描画(平行光源)
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="camera">カメラ</param>
		/// <param name="animationTime">アニメーションの時間</param>
		/// <param name="material">マテリアル</param>
		/// <param name="directionalLight">平行光源</param>
		void Draw(const WorldTransform& worldTransform, const Camera& camera, const float& animationTime, const Material& material, const DirectionalLight& directionalLight);

		/// <summary>
		/// 描画(点光源)
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="camera">カメラ</param>
		/// <param name="animationTime">アニメーションの時間</param>
		/// <param name="material">マテリアル</param>
		/// <param name="pointLight">点光源</param>
		void Draw(const WorldTransform& worldTransform, const Camera& camera, const float& animationTime, const Material& material, const PointLight& pointLight);

		/// <summary>
		/// 描画(スポットライト)
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="camera">カメラ</param>
		/// <param name="animationTime">アニメーションの時間</param>
		/// <param name="material">マテリアル</param>
		/// <param name="spotLight">スポットライト</param>
		void Draw(const WorldTransform& worldTransform, const Camera& camera, const float& animationTime, const Material& material, const SpotLight& spotLight);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~AnimationModel() = default;


	public:
		/// <summary>
		/// 環境マップで使うテクスチャのの設定
		/// </summary>
		/// <param name="textureHandle"></param>
		void SetEnvironmentTexture(const uint32_t& textureHandle) {
			this->environmentTextureHandle_ = textureHandle;
		}


	private:
		//DirectXのクラス
		Elysia::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Elysia::PipelineManager* pipelineManager_ = nullptr;
		//SRV管理クラス
		Elysia::SrvManager* srvManager_ = nullptr;
		//テクスチャ管理クラス
		Elysia::TextureManager* textureManager_ = nullptr;
		//モデル管理クラス
		Elysia::ModelManager* modelManager_ = nullptr;
		//アニメーション管理クラス
		Elysia::AnimationManager* animationManager_ = nullptr;

	private:
		//時間変化の量
		const float_t DELTA_TIME_ = 1.0f / 60.0f;
	private:
		//頂点リソースを作る
		ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		//頂点バッファビューを作成する
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

		//インデックス
		ComPtr<ID3D12Resource> indexResource_ = nullptr;
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		//PixelShaderにカメラの座標を送る為の変数
		ComPtr<ID3D12Resource> cameraResource_ = nullptr;
		CameraForGPU* cameraForGPU_ = {};


		//テクスチャハンドル
		uint32_t textureHandle_ = 0u;
		//環境マップ
		uint32_t environmentTextureHandle_ = 0;
		//モデルハンドル
		uint32_t modelHandle_ = 0u;
		ModelData modelData_ = {};

		//アニメーションハンドル
		uint32_t animationHandle_ = 0u;
		//アニメーションのローカル座標
		Matrix4x4 animationLocalMatrix_ = {};
		//スケルトン
		Skeleton humanSkeleton_ = {};
		//スキンクラスター
		SkinCluster skinCluster_ = {};


	};
};



