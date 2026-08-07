#include "HighPassLongNote.h"
#include <imgui.h>

void HighPassLongNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_ = Elysia::Model::Create(modelHandle);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.4f;
	worldTransform_.scale.y = 3.0;
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = LightingType::DirectionalLighting;
	material_.color = { .x = 1.0f,.y = 0.4f,.z = 0.6f,.w = 1.0f };
}

void HighPassLongNote::Update(){


	//開始の比率を計算
	ratio_ = SingleCalculation::InverseLerp(startMoveTime_, arriveLineTime_, musicTime_);

	//始点座標
	float_t startPositionX = SingleCalculation::Lerp(initialPositionX_, judgmentPositionX_, ratio_);
	//終点座標
	float_t endPositionX = SingleCalculation::Lerp(initialPositionX_, judgmentPositionX_, endRatio_);
	//半分の所でスケールを伸ばしていきたい
	worldTransform_.translate.x= (startPositionX + endPositionX) / 2.0f;
	worldTransform_.translate.y = lanePositionY_;

	//スケールの計算
	//裏返り厳禁！
	worldTransform_.scale.x = (abs(endPositionX - startPositionX) / 2.0f)*1.1f;

	//最後までいったら未使用状態にする
	if (endRatio_ >= 1.0f) {
		isUsed_ = false;
	}

#ifdef _DEBUG
	ImGui::Begin("ハイパス用ロングノーツ");
	ImGui::SliderFloat("AnchorX", &worldTransform_.anchorPoint.x, -1.0f, 1.0f);
	ImGui::SliderFloat("ScaleX", &worldTransform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Color", &material_.color.x, 0.0f, 1.0f);
	ImGui::InputFloat("EndPositionX", &endPositionX);
	ImGui::End();
#endif // _DEBUG


	//更新
	worldTransform_.Update();
	material_.Update();
}

