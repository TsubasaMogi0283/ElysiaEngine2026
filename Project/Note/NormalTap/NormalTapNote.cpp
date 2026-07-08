#include "NormalTapNote.h"



void NormalTapNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_= Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0f;
	//座標の初期化
	worldTransform_.translate.z = 0.0f;
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
}

void NormalTapNote::Update(){
	ratio_ = SingleCalculation::InverseLerp(startMoveTime_, arriveLineTime_, musicTime_);
	//座標の計算
	worldTransform_.translate.x = SingleCalculation::Lerp(initialPositionX_, judgmentPositionX_, ratio_);
	worldTransform_.translate.y = lanePositionY_;

	//使用状態の更新
	if (ratio_ >= 1.0f) {
		ratio_ = 0.0f;
		isUsed_ = false;
	}

	//更新
	worldTransform_.Update();
	material_.Update();
}

void NormalTapNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	if (isUsed_) {
		model_->Draw(worldTransform_, camera, material_, baseLight);
	}
}
