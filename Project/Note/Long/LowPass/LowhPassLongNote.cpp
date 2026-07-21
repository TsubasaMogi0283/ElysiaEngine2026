#include "LowPassLongNote.h"
#include <imgui.h>

void LowPassLongNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_ = Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0;
	worldTransform_.anchorPoint.x = -1.0f;
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
	material_.color = { .x = 0.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
}

void LowPassLongNote::Update(){
#ifdef _DEBUG
	ImGui::Begin("LowLongNote");
	ImGui::SliderFloat("AnchorX", &worldTransform_.anchorPoint.x, -1.0f, 1.0f);
	ImGui::SliderFloat("ScaleX", &worldTransform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Color", &material_.color.x, 0.0f, 1.0f);

	ImGui::End();
#endif // _DEBUG


	//更新
	worldTransform_.Update();
	material_.Update();
}

