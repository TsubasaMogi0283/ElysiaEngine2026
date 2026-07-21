#include "BaseLongNote.h"
#include <imgui.h>

void BaseLongNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
