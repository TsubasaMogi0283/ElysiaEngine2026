#include "BaseLongNote.h"
#include <imgui.h>

void BaseLongNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//描画部分は共通なのでここでやる
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
