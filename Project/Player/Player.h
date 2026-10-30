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
/// 判定線
/// </summary>
class JudgementLine;

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
	void Initialize(const uint32_t& modelHandle,const std::vector<uint32_t>& animationHandle);

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

public:
	/// <summary>
	/// アニメーション
	/// </summary>
	enum class AnimationSelection {
		//待機
		Wait,
		//走る
		Run,

		//メインシーンにて
		//パンチ(上)
		Punch,
		//キック(下)
		Kick,
	};

private:
	//アニメーション管理クラス
	Elysia::AnimationManager* animationManager_ = nullptr;

private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;


public:

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const Vector3<float_t>& scale) {
		this->worldTransform_.scale = scale;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotate">回転</param>
	inline void SetRotate(const Vector3<float_t>& rotate) {
		this->worldTransform_.rotate = rotate;
	}

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetPosition(const Vector3<float_t>& position) {
		this->worldTransform_.translate = position;
	}

	/// <summary>
	/// 判定線の取得
	/// </summary>
	/// <param name="judgementLine">判定線</param>
	void SetJudgementLine(JudgementLine* judgementLine);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	inline Vector3<float_t> GetWorldPosition()const {
		return worldTransform_.GetWorldPosition();
	}

private:
	//判定線
	JudgementLine* judgementLine_ = nullptr;

private:
	//モデル
	std::unique_ptr<Elysia::AnimationModel> animationModel_ = nullptr;
	//アニメーションのハンドル
	std::vector<uint32_t>animationHandleVector_ = {};

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};

	//アニメーション時間
	float_t animationTime_ = 0.0f;


};

