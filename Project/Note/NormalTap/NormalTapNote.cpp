#include "NormalTapNote.h"



void NormalTapNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_= Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0f;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
}

void NormalTapNote::Update(){

	//座標の計算
	worldTransform_.translate.x = SingleCalculation::Lerp(initialPositionX_, judgmentPositionX_, ratio_);
	worldTransform_.translate.y = 0.0f;
	worldTransform_.translate.z = 0.0f;

	//使用状態の更新
	if (ratio_ >= 1.2f) {
		ratio_ = 0.0f;
		isUsed_ = false;
	}

	//更新
	worldTransform_.Update();
	material_.Update();
}

void NormalTapNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
