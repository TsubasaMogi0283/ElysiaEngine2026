#pragma once

/**
 * @file Player.h
 * @brief プレイヤーのクラス
 * @author 茂木翼
 */

#include <AnimationModel.h>
#include <Material.h>


/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// 基底ライト
/// </summary>
struct BaseLight;

/// <summary>
/// プレイヤー
/// </summary>
class Player{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="animationHandle"></param>
	void Initialize(const uint32_t& modelHandle,const uint32_t& animationHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() = default;

private:
	//モデル
	std::unique_ptr<Elysia::AnimationModel> animationModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};


	float_t animationTime_ = 0.0f;

};

