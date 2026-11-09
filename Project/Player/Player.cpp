#include "Player.h"

#include <AnimationManager.h>
#include <JudgementLine/JudgementLine.h>

Player::Player(){
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
}

void Player::Initialize(const uint32_t& modelHandle, const std::vector<uint32_t>& animationHandle){
	
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

}

void Player::Update(){

	//アニメーションの実行
	(this->*functionTable[static_cast<uint8_t>(currentState_)])();

	//更新
	worldTransform_.Update();
	material_.Update();
	
}

void Player::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//モデルの描画
	animationModel_->Draw(worldTransform_, camera,runAnimationTime_ ,material_, baseLight);
}

void Player::Wait(){

}

void Player::Run(){
	//アニメーションの時間を設定
	runAnimationTime_ += DELTA_TIME_;
}

void Player::Fall(){
	//落下
	worldTransform_.translate.y -= 0.2f;

	//地面に到達したら落下を止める
	if (worldTransform_.translate.y < GROUND_POSITION_Y_) {
		worldTransform_.translate.y = GROUND_POSITION_Y_;
		currentState_ = AnimationState::Run;
	}
}

void Player::Punch(){
	//下側に移動
	worldTransform_.translate.y = judgementLine_->GetLanePosition()[static_cast<uint8_t>(NoteLane::Place::Down)]+ OFFSET_POSITION_Y_;

	//アニメーション時間の加算
	punchAnimationTime_ += DELTA_TIME_;

	//動作が終わったら走る動作へ
	if (punchAnimationTime_ >= maxPunchAnimationTime_) {
		punchAnimationTime_ = 0.0f;
		currentState_ = AnimationState::Fall;
	}
}

void Player::Kick(){
	//上側に移動
	worldTransform_.translate.y = judgementLine_->GetLanePosition()[static_cast<uint8_t>(NoteLane::Place::Up)]+ OFFSET_POSITION_Y_;

	//アニメーション時間の加算
	kickAnimationTime_ += DELTA_TIME_;

	//動作が終わったら走る動作へ
	if (kickAnimationTime_ >= maxKickAnimationTime_) {
		kickAnimationTime_ = 0.0f;
		currentState_ = AnimationState::Fall;
	}
}

void Player::SetJudgementLine(JudgementLine* judgementLine){
	this->judgementLine_ = judgementLine;
}
