#include "LongNote.h"

LongNote::LongNote(){

}

void LongNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_ = Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
}

void LongNote::Update(){

	//更新
	worldTransform_.Update();
	material_.Update();
}

void LongNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
