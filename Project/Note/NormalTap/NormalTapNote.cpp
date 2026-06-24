#include "NormalTapNote.h"

NormalTapNote::NormalTapNote(){

}

void NormalTapNote::Initialize(const uint32_t& modelHandle){
	//モデルの生成
	model_=Elysia::Model::Create(modelHandle);
}

void NormalTapNote::Update(){
	//更新
	worldTransform_.Update();
	material_.Update();
}

void NormalTapNote::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
