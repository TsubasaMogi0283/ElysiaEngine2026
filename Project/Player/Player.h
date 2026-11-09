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
	/// パンチ
	/// </summary>
	void Punch();

	/// <summary>
	/// キック
	/// </summary>
	void Kick();

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
		&Player::Punch,
		&Player::Kick,
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


		//余裕があれば片腕片足だけではなく両方対応できるようにしたい
		//パンチ(上)
		Punch,
		//キック(下)
		Kick,
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
	/// 上攻撃(パンチ)
	/// </summary>
	inline void SetUpAttack() {
		currentState_ = AnimationState::Punch;
		//animationHandleVector_[static_cast<uint8_t>(AnimationState::Punch)];
	}

	/// <summary>
	/// 下攻撃(キック)
	/// </summary>
	inline void SetDownAttack() {
		currentState_ = AnimationState::Kick;
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

private:
	//モデル
	std::unique_ptr<Elysia::AnimationModel> animationModel_ = nullptr;
	//アニメーションのハンドル
	std::vector<uint32_t>animationHandleVector_ = {};
	uint32_t currentAnimationHandle_ = 0u;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
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

};