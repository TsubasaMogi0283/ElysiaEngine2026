#include "JudgementLine.h"

void JudgementLine::Initialize(uint32_t normalNoteModelHandle){

	//モデルの読み込み
	model_ = Elysia::Model::Create(normalNoteModelHandle);
	worldTransform_.Initialize();
	worldTransform_.scale.x = 0.1f;
	worldTransform_.scale.y = 10.0f;
	worldTransform_.translate.x = JUDGEMENT_POSITION_X_;
	worldTransform_.translate.y = 5.0f;
	//判定線のマテリアルを生成
	material_.Initialize();
	material_.color = { .x = 1.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
}

void JudgementLine::Update(){
	//更新
	worldTransform_.Update();
	material_.Update();
}

void JudgementLine::Draw(const Camera& camera, const BaseLight& baseLight){
	model_->Draw(worldTransform_, camera, material_, baseLight);
}
