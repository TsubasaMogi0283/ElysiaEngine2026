#pragma once

/**
 * @file Player.h
 * @brief プレイヤーのクラス
 * @author 茂木翼
 */

#include <AnimationModel.h>
#include <Material.h>
#include <Note/NoteLane.h>


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

private:
	/// <summary>
	/// 待機
	/// </summary>
	void Wait();

	/// <summary>
	/// 走る
	/// </summary>
	void Run();

	/// <summary>
	/// 落ちる
	/// </summary>
	void Fall();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public:

	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (Player::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (Player::* functionTable[])() = {
		&Player::Wait,
		&Player::Run,
		&Player::Fall,
		&Player::Attack
	};

	/// <summary>
	/// アニメーション状態
	/// </summary>
	enum class AnimationState {
		//待機
		Wait,
		//走る
		Run,
		//落ちる
		Fall,

		//攻撃
		Attack
	};

	//現在の状態
	AnimationState currentState_ = AnimationState::Run;

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

	/// <summary>
	/// 攻撃の設定
	/// </summary>
	/// <param name="place">位置</param>
	inline void SetAttack(const NoteLane::Place& place) {
		//攻撃状態にする
		this->currentState_ = AnimationState::Attack;
		//攻撃位置の設定
		this->attackPlace_ = place;
		//ジャンプの設定
		this->isJump_ = true;
	}

private:
	//アニメーション管理クラス
	Elysia::AnimationManager* animationManager_ = nullptr;
	//判定線
	JudgementLine* judgementLine_ = nullptr;

private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;

	//地面のY座標
	const float_t GROUND_POSITION_Y_ = 0.0f;
	//攻撃時の座標のオフセット
	const float_t OFFSET_POSITION_Y_ = -2.5f;

	//初期の打ち上げ速度
	const float_t INITIAL_LAUNCH_VELOCITY_Y_ = 0.5f;

private:
	//モデル
	std::unique_ptr<Elysia::AnimationModel> animationModel_ = nullptr;
	//アニメーションのハンドル
	std::vector<uint32_t>animationHandleVector_ = {};
	uint32_t currentAnimationHandle_ = 0u;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//ジャンプ(攻撃)したかどうか
	bool isJump_ = false;
	//ジャンプ時の初期設定
	bool isInitializeJumpPosition_ = false;

	//加速の合計値
	float_t velocityAccelerationY_ = INITIAL_LAUNCH_VELOCITY_Y_;
	//ジャンプ時の加速度
	const float_t JUMP_ACCELERATION_Y_ = -0.05f;
	//マテリアル
	Material material_ = {};

	//アニメーション時間
	float_t runAnimationTime_ = 0.0f;
	float_t maxRunAnimationTime_ = 0.5f;
	//キックのアニメーション時間
	float_t kickAnimationTime_ = 0.0f;
	float_t maxKickAnimationTime_ = 0.5f;
	//パンチのアニメーション時間
	float_t punchAnimationTime_ = 0.0f;
	float_t maxPunchAnimationTime_ = 0.5f;

	//攻撃位置
	NoteLane::Place attackPlace_ = NoteLane::Place::Size;
};