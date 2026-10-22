#include "Player.h"

#include <AnimationManager.h>
#include <JudgementLine/JudgementLine.h>

Player::Player(){
	//アニメーション管理クラス
	animationManager_ = Elysia::AnimationManager::GetInstance();
}

void Player::Initialize(const uint32_t& modelHandle, const std::vector<uint32_t>& animationHandle){
	

	//モデルの生成
	//待機アニメーションで初期化
	animationModel_ = Elysia::AnimationModel::Create(modelHandle, animationHandle[static_cast<uint8_t>(AnimationSelection::Wait)]);

	//ワールドトランスフォーム
	//初期化
	worldTransform_.Initialize();
	//マテリアル
	//初期化
	material_.Initialize();


}

void Player::Update(){
	//アニメーションの時間を設定
	animationTime_ += DELTA_TIME_;

	//更新
	worldTransform_.Update();
	material_.Update();
	
}

void Player::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	animationModel_->Draw(worldTransform_, camera,animationTime_ ,material_, baseLight);
}

void Player::SetJudgementLine(JudgementLine* judgementLine){
	this->judgementLine_ = judgementLine;
}
