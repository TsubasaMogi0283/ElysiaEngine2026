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
	inline void SetPosition(const Vector3& position) {
		this->worldTransform_.translate = position;
	}

	//使用状態を設定
	inline void SetIsUsed(const bool& isUsed) {
		this->isUsed_ = isUsed;
	}

	/// <summary>
	/// 使用状態を取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsUsed()const {
		return isUsed_;
	}

	/// <summary>
	/// 移動の比率を設定
	/// </summary>
	/// <param name="ratio"></param>
	inline void SetRatio(const float_t& ratio) {
		this->ratio_ = ratio;
	}

	/// <summary>
	/// 移動の比率を取得
	/// </summary>
	/// <returns></returns>
	inline float_t GetRatio()const {
		return ratio_;
	}

	/// <summary>
	/// 初期座標
	/// </summary>
	/// <param name="position"></param>
	inline void SetInitialPosition(const Vector3& position) {
		this->initialPosition_ = position;
	}

	/// <summary>
	/// 判定の座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetJudgmentPosition(const Vector3& position) {
		this->judgmentPosition_ = position;
	}

protected:
	//モデル
	std::unique_ptr<Elysia::Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
	//使用状態
	bool isUsed_ = false;
	//動きの比率
	float_t ratio_ = 0.0f;

	//初期座標
	float_t initialPositionX_ = 0.0f;
	//判定座標
	float_t judgmentPositionX_ = 0.0f;

};

