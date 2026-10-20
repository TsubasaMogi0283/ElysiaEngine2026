#include "Player.h"

Player::Player(){

}

void Player::Initialize(const uint32_t& modelHandle, const uint32_t& animationHandle){
	//モデルの生成
	animationModel_ = Elysia::AnimationModel::Create(modelHandle, animationHandle);

	//ワールドトランスフォーム
	//初期化
	worldTransform_.Initialize();

	//マテリアル
	//初期化
	material_.Initialize();


}

void Player::Update(){

	worldTransform_.Update();
	material_.Update();
}

void Player::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	animationModel_->Draw(worldTransform_, camera,animationTime_ ,material_, baseLight);
}
