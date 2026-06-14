#include "NormalTapNote.h"

NormalTapNote::NormalTapNote(){

}

void NormalTapNote::Initialize(const uint32_t modelHandle){
	//モデルの生成
	model_=Elysia::Model::Create(modelHandle);
}

void NormalTapNote::Update()
{
}

void NormalTapNote::DrawObject3D(const Camera& camera)
{

	model_->Draw()
}
