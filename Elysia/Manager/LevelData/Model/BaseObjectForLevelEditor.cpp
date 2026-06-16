#include "BaseObjectForLevelEditor.h"

void BaseObjectForLevelEditor::Draw(const Camera& camera){
	//ライティング無しに設定
	material_.lightingKinds = LightingType::NoneLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_);
}

void BaseObjectForLevelEditor::Draw(const Camera& camera, const BaseLight& baseLight){
	//平行光源に設定
	material_.lightingKinds = LightingType::DirectionalLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
