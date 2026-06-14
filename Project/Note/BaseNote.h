#pragma once

/**
 * @file BaseNote.h
 * @brief ノーツの基底クラス
 * @author 茂木翼
 */

#include <WorldTransform.h>
#include <Model.h>

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
	virtual void Initialize(const uint32_t modelHandle) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void DrawObject3D(const Camera& camera) = 0;

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
		this->worldTransform_.translate;
	}

protected:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//モデル
	std::unique_ptr<Elysia::Model>model_ = nullptr;

};

