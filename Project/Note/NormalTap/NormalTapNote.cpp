#include "NormalTapNote.h"



void NormalTapNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_= Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
}

void NormalTapNote::Update(){
	//更新
	worldTransform_.Update();
	material_.Update();
}

void NormalTapNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
