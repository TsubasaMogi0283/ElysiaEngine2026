#pragma once

/**
 * @file BaseNote.h
 * @brief ノーツの基底クラス
 * @author 茂木翼
 */

#include <Model.h>
#include <WorldTransform.h>
#include <Material.h>


/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// ノーツの基底クラス
/// </summary>
class BaseNote{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	virtual void Initialize(const uint32_t& modelHandle) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	virtual void DrawObject3D(const Camera& camera,const BaseLight& baseLight) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseNote() = default;

public:
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) {
		this->worldTransform_.translate = position;
	}

protected:
	//モデル
	std::unique_ptr<Elysia::Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
};

