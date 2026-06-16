#include "BaseObjectForLevelEditor.h"
#include <BaseLight.h>

void BaseObjectForLevelEditor::Draw(const Camera& camera){
	//ライティング無しに設定
	material_.lightingKinds = LightingType::NoneLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_);
}

void BaseObjectForLevelEditor::Draw(const Camera& camera, const BaseLight& baseLight){
	switch (baseLight.lightingType) {
	default:
		material_.lightingKinds = LightingType::NoneLighting;
		break;

	case LightingType::DirectionalLighting:
		//マテリアルの方に平行光源に設定
		material_.lightingKinds = LightingType::DirectionalLighting;
		break;

	case LightingType::PointLighting:
		//マテリアルの方に点光源に設定
		material_.lightingKinds = LightingType::PointLighting;
		break;

	case LightingType::SpotLighting:
		//マテリアルの方にスポットライトに設定
		material_.lightingKinds = LightingType::SpotLighting;
		break;

	}
	
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
