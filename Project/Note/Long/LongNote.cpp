#include "LongNote.h"
#include <imgui.h>

LongNote::LongNote(){

}

void LongNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_ = Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0;
	worldTransform_.anchorPoint = {};
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
}

void LongNote::Update(){

#ifdef _DEBUG
	ImGui::Begin("LongNote");
	ImGui::SliderFloat("AnchorX", &worldTransform_.anchorPoint.x,-1.0f,1.0f);
	ImGui::SliderFloat("ScaleX", &worldTransform_.scale.x,1.0f,10.0f);
	ImGui::End();
#endif // _DEBUG


	//更新
	worldTransform_.Update();
	material_.Update();
}

void LongNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
