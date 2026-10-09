#include "GameNameObject.h"

void GameNameObject::Initialize(const uint32_t& dModelhandle, const uint32_t& aModelHandle, const uint32_t& pModelHandle, const uint32_t& exclamationModelhandle){
	//各オブジェクトの初期化
	//D
	for (uint8_t i = 0u;i < 2u;i++) {
		GenerateTextObject(dObjectArray_[i], dModelhandle);
	}
	//A
	for (uint8_t i = 0u;i < 3u;i++) {
		GenerateTextObject(aObjectArray_[i], aModelHandle);
	}
	//P
	GenerateTextObject(pObject_, pModelHandle);
	//!
	for (uint8_t i = 0u;i < 3u;i++) {
		GenerateTextObject(exclamationObjectArray_[i], pModelHandle);
	}
}

void GameNameObject::Update(){

}

void GameNameObject::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//Dのモデル
	for (uint8_t i = 0u;i < D_OBJECT_AMOUNT_;i++) {
		dObjectArray_[i].model_->Draw(dObjectArray_[i].worldTransform, camera, dObjectArray_[i].material);
	}

	//Aのモデル
	for (uint8_t i = 0u;i < A_OBJECT_AMOUNT_;i++) {
		aObjectArray_[i].model_->Draw(aObjectArray_[i].worldTransform, camera, aObjectArray_[i].material);
	}
	//Pのモデル
	pObject_.model_->Draw(pObject_.worldTransform, camera, pObject_.material);
	//!のモデル
	for (uint8_t i = 0u;i < EXCLAMATION_OBJECT_AMOUNT_;i++) {
		exclamationObjectArray_[i].model_->Draw(exclamationObjectArray_[i].worldTransform, camera, exclamationObjectArray_[i].material);
	}
	

}

void GameNameObject::GenerateTextObject(TextObjectInformation& objectInformation, const uint32_t modelHandle){
	objectInformation.model_ = Elysia::Model::Create(modelHandle);
	objectInformation.worldTransform.Initialize();
	objectInformation.material.Initialize();

}

void GameNameObject::UpdateTextObject(TextObjectInformation& objectInformation, const uint32_t modelHandle){
	objectInformation.worldTransform.Update();
	objectInformation.material.Update();
}
