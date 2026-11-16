#include "Player.h"

#include <AnimationManager.h>
#include <JudgementLine/JudgementLine.h>

Player::Player() {
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
}

void Player::Initialize(const uint32_t& modelHandle, const std::vector<uint32_t>& animationHandle) {

	//初期状態は走る
	currentState_ = AnimationState::Run;
	//アニメーションハンドルの設定
	animationHandleVector_ = animationHandle;
	//currentAnimationHandle_ = animationHandleVector_[static_cast<uint8_t>(currentState_)];
	currentAnimationHandle_ = animationHandleVector_[0u];

	//モデルの生成
	//待機アニメーションで初期化
	animationModel_ = Elysia::AnimationModel::Create(modelHandle, currentAnimationHandle_);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//マテリアルの初期化
	material_.Initialize();

	isJump_ = false;
	worldTransform_.translate.y = GROUND_POSITION_Y_;
}

void Player::Update() {

	//アニメーションの実行
	(this->*functionTable[static_cast<uint8_t>(currentState_)])();

	//ジャンプ時
	if (isJump_) {
		//落下
		velocityAccelerationY_ += JUMP_ACCELERATION_Y_;
		worldTransform_.translate.y += velocityAccelerationY_;
	}
	else {
		worldTransform_.translate.y = GROUND_POSITION_Y_;
	}

	//
	if (worldTransform_.translate.y < GROUND_POSITION_Y_) {
		worldTransform_.translate.y = GROUND_POSITION_Y_;
		isGround_ = true;
	}

	//更新
	worldTransform_.Update();
	material_.Update();

}

void Player::DrawObject3D(const Camera& camera, const BaseLight& baseLight) {
	//モデルの描画
	animationModel_->Draw(worldTransform_, camera, runAnimationTime_, material_, baseLight);
}

void Player::Wait() {

}

void Player::Run() {
	//アニメーションの時間を設定
	runAnimationTime_ += DELTA_TIME_;
}

void Player::Fall() {
	//地面に到達したら落下を止める
	if (isGround_) {
		currentState_ = AnimationState::Run;
		//初期値に設定
		velocityAccelerationY_ = INITIAL_LAUNCH_VELOCITY_Y_;
		//ジャンプの初期化
		isJump_ = false;
	}
}

void Player::Attack() {
	//アニメーション時間の加算
	punchAnimationTime_ += DELTA_TIME_;

	//動作が終わったら走る動作へ
	if (punchAnimationTime_ >= maxPunchAnimationTime_) {
		punchAnimationTime_ = 0.0f;
		currentState_ = AnimationState::Fall;
		attackPlace_ = NoteLane::Place::Size;
	}
}

void Player::SetJudgementLine(JudgementLine* judgementLine) {
	this->judgementLine_ = judgementLine;
}

void Player::SetAttack(const NoteLane::Place& place){
	//攻撃状態にする
	currentState_ = AnimationState::Attack;
	//攻撃位置の設定
	attackPlace_ = place;
	//ジャンプの設定
	isJump_ = true;
	//空中にいる設定
	isGround_ = false;
	//初期値に戻す
	worldTransform_.translate.y = judgementLine_->GetLanePosition()[static_cast<uint8_t>(attackPlace_)] + OFFSET_POSITION_Y_;

	//落下
	velocityAccelerationY_ = INITIAL_LAUNCH_VELOCITY_Y_;
}
