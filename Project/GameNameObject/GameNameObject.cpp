#include "GameNameObject.h"

void GameNameObject::Initialize(const uint32_t& dModelhandle, const uint32_t& aModelHandle, const uint32_t& pModelHandle, const uint32_t& exclamationModelhandle){
	//各オブジェクトの初期化
	//D
	for (uint8_t i = 0u;i < D_OBJECT_AMOUNT_;i++) {
		GenerateTextObject(dObjectArray_[i], dModelhandle,static_cast<TextObjectNumber>(i));
	}
	//A
	for (uint8_t i = 0u;i < A_OBJECT_AMOUNT_;i++) {
		GenerateTextObject(aObjectArray_[i], aModelHandle, static_cast<TextObjectNumber>(i));
	}
	//P
	GenerateTextObject(pObject_, pModelHandle, TextObjectNumber::First);
	//!
	for (uint8_t i = 0u;i < EXCLAMATION_OBJECT_AMOUNT_;i++) {
		GenerateTextObject(exclamationObjectArray_[i], exclamationModelhandle, static_cast<TextObjectNumber>(i));
	}
}

void GameNameObject::Update(){



	//DA! DA! PA!




	//各オブジェクトの更新
	//D
	for (uint8_t i = 0u; i < D_OBJECT_AMOUNT_; i++) {
		UpdateTextObject(dObjectArray_[i]);
	}
	//A
	for (uint8_t i = 0u; i < A_OBJECT_AMOUNT_; i++) {
		UpdateTextObject(aObjectArray_[i]);
	}
	//P
	UpdateTextObject(pObject_);
	//!
	for (uint8_t i = 0u; i < EXCLAMATION_OBJECT_AMOUNT_; i++) {
		UpdateTextObject(exclamationObjectArray_[i]);
	}
}

void GameNameObject::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//Dのモデル
	for (uint8_t i = 0u;i < D_OBJECT_AMOUNT_;i++) {
		dObjectArray_[i].model_->Draw(dObjectArray_[i].worldTransform, camera, dObjectArray_[i].material, baseLight);
	}

	//Aのモデル
	for (uint8_t i = 0u;i < A_OBJECT_AMOUNT_;i++) {
		aObjectArray_[i].model_->Draw(aObjectArray_[i].worldTransform, camera, aObjectArray_[i].material, baseLight);
	}
	//Pのモデル
	pObject_.model_->Draw(pObject_.worldTransform, camera, pObject_.material, baseLight);
	//!のモデル
	for (uint8_t i = 0u;i < EXCLAMATION_OBJECT_AMOUNT_;i++) {
		exclamationObjectArray_[i].model_->Draw(exclamationObjectArray_[i].worldTransform, camera, exclamationObjectArray_[i].material, baseLight);
	}
	
}

void GameNameObject::GenerateTextObject(TextObjectInformation& objectInformation, const uint32_t& modelHandle, const TextObjectNumber& number){
	//モデルの生成
	objectInformation.model_ = Elysia::Model::Create(modelHandle);
	objectInformation.worldTransform.Initialize();
	objectInformation.material.Initialize();
	objectInformation.number = number;

}

void GameNameObject::UpdateTextObject(TextObjectInformation& objectInformation){
	objectInformation.worldTransform.Update();
	objectInformation.material.Update();
}
