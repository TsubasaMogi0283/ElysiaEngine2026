#include "GameNameObject.h"

#include <SingleCalculation.h>
#include <Easing.h>

void GameNameObject::Initialize(const uint32_t& dModelHandle, const uint32_t& aModelHandle, const uint32_t& pModelHandle, const uint32_t& exclamationModelHandle){
	//D
	for (uint8_t i = 0u;i < D_OBJECT_AMOUNT_;i++) {
		wordArray_[i].objectArray[static_cast<uint8_t>(WordNumber::First)].model = Elysia::Model::Create(dModelHandle);
	}
	//P
	wordArray_[static_cast<uint8_t>(WordNumber::Third)].objectArray[static_cast<uint8_t>(WordNumber::First)].model = Elysia::Model::Create(pModelHandle);
	//A
	for (uint8_t i = 0u;i < A_OBJECT_AMOUNT_;i++) {
		wordArray_[i].objectArray[static_cast<uint8_t>(WordNumber::Second)].model = Elysia::Model::Create(aModelHandle);
	}
	//!
	for (uint8_t i = 0u;i < EXCLAMATION_OBJECT_AMOUNT_;i++) {
		wordArray_[i].objectArray[static_cast<uint8_t>(WordNumber::Third)].model = Elysia::Model::Create(exclamationModelHandle);
	}


	//各オブジェクトの初期化
	for (uint8_t i = 0u;i < WORD_AMOUNT_;i++) {
		for (uint8_t j = 0u; j < TEXT_AMOUNT_; j++) {
			wordArray_[i].objectArray[j].worldTransform.Initialize();
			wordArray_[i].objectArray[j].material.Initialize();
		}

		//移動時間の設定
		wordArray_[i].startMoveTime = i * 2.0f;
		wordArray_[i].endMoveTime = wordArray_[i].startMoveTime + TEXT_OBJECT_MOVE_TIME_;
		//単語の番号の設定
		wordArray_[i].number = static_cast<WordNumber>(i);
	}
}

void GameNameObject::Update(){
	//動き始めの指示があったら更新開始
	if (isMove_&&!isProcessEnd_) {
		for (uint8_t i = 0u; i < WORD_AMOUNT_; i++) {
			for (uint8_t j = 0u; j < TEXT_AMOUNT_; j++) {
				//回転と座標移動
				wordArray_[i].moveTime += DELTA_TIME_;
				float_t t = SingleCalculation::InverseLerp(wordArray_[i].startMoveTime, wordArray_[i].endMoveTime, wordArray_[i].moveTime);
				t = std::clamp(t, 0.0f, 1.0f);
				float_t easeT = Easing::EaseInQuart(t);

				//Y軸回転
				wordArray_[i].objectArray[j].worldTransform.rotate.y = SingleCalculation::Lerp(INITIAL_ROTATE_Y_, FINALE_ROTATE_Y_, easeT);
				//Z軸移動
				wordArray_[i].objectArray[j].worldTransform.translate.z = SingleCalculation::Lerp(INITIAL_POSITION_Z_, FINALE_POSITION_Z_, easeT);
			}
		}

		//最後の単語も移動が終わったら処理終了
		if (wordArray_[static_cast<uint8_t>(WordNumber::Third)].moveTime > wordArray_[static_cast<uint8_t>(WordNumber::Third)].endMoveTime) {
			isProcessEnd_ = true;
		}
	}

	//更新
	for (uint8_t i = 0u; i < WORD_AMOUNT_; i++) {
		for (uint8_t j = 0u; j < TEXT_AMOUNT_; j++) {
			wordArray_[i].objectArray[j].worldTransform.Update();
			wordArray_[i].objectArray[j].material.Update();
		}
	}
}

void GameNameObject::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//各オブジェクトの描画
	for (uint8_t i = 0u; i < WORD_AMOUNT_; i++) {
		for (uint8_t j = 0u; j < TEXT_AMOUNT_; j++) {
			wordArray_[i].objectArray[j].model->Draw(
				wordArray_[i].objectArray[j].worldTransform,camera, wordArray_[i].objectArray[j].material);
		}
	}
}
