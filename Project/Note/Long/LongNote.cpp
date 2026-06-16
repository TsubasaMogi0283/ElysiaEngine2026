#include "LongNote.h"

LongNote::LongNote(){

}

void LongNote::Initialize(const uint32_t modelHandle){
	//モデルの生成
	model_=Elysia::Model::Create(modelHandle);

	worldTransform_.Initialize();
	material_.Update();
}

void LongNote::Update()
{
	//更新
	worldTransform_.Update();
	material_.Update();
}

void LongNote::DrawObject3D(const Camera& camera)
{
	camera;
	//model_->Draw()
}
