#include "HighPassLongNote.h"
#include <imgui.h>

void HighPassLongNote::Initialize(const uint32_t& modelHandle){
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
	material_.color = { .x = 1.0f,.y = 0.4f,.z = 0.6f,.w = 1.0f };
}

void HighPassLongNote::Update(){
#ifdef _DEBUG
	ImGui::Begin("LongNote");
	ImGui::SliderFloat("AnchorX", &worldTransform_.anchorPoint.x, -1.0f, 1.0f);
	ImGui::SliderFloat("ScaleX", &worldTransform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Color", &material_.color.x, 0.0f, 1.0f);
	ImGui::InputFloat("EndPositionX", &endPositionX_);
	ImGui::End();
#endif // _DEBUG

	//開始の比率を計算
	ratio_ = SingleCalculation::InverseLerp(startMoveTime_, arriveLineTime_, musicTime_);

	
	//座標の計算
	worldTransform_.translate.x = SingleCalculation::Lerp(initialPositionX_, judgmentPositionX_, ratio_);
	worldTransform_.translate.y = lanePositionY_;

	//スケールの計算
	worldTransform_.scale.x = endPositionX_ - worldTransform_.translate.x;


	//使用状態の更新
	if (ratio_ >= 1.0f) {
		//ratio_ = 0.0f;
		//isUsed_ = false;
	}

	//更新
	worldTransform_.Update();
	material_.Update();
}

