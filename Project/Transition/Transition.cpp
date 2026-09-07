#include "Transition.h"
#include <Windows/WindowsSetup.h>
#include <TextureManager.h>
#include <Easing.h>


void Transition::Initialize(){
	//テクスチャの読み込み
	textureHandle_ = Elysia::TextureManager::GetInstance()->Load("Resources/Sprite/Back/White.png");

	//テクスチャの縦幅を取得
	uint64_t textureHeight = Elysia::TextureManager::GetInstance()->GetTextureHeight(textureHandle_);
	heightScale_ = static_cast<float_t>(DIVIDE_NUMBER_)/ static_cast<float_t>(textureHeight);
	//スプライトの生成
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;++i) {
		spriteArray_[i] = Elysia::Sprite::Create(textureHandle_);
		spriteArray_[i]->SetScale({ .x = widthScale_, .y = heightScale_ });
		spriteArray_[i]->SetPosition({ .x = 0u, .y = static_cast<uint16_t>(i * textureHeight / DIVIDE_NUMBER_) });


		//偶数列の時は右から
		if (i % 2 == 0) {

		}
	}
}

void Transition::Update(){

	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;++i) {
		spriteArray_[i]->SetScale({ .x = widthScale_, .y = heightScale_ });
	}

}

void Transition::DrawSprite(){
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;++i) {
		spriteArray_[i]->Draw();
	}
}

bool Transition::SetOpenTransition(){

	bool isEnd = false;

	//閉じている状態の時に開く
	if (!isOpened_&& isClosed_) {
		
		//イージングで滑らかに開ける
		openT_ += DELTA_TIME_;
		widthScale_ = Easing::EaseInOutQuad(openT_);
		widthScale_ = std::clamp(widthScale_, 0.0f, 1.0f);

		//最大値になったら開いた状態にする
		if (openT_ >= 1.0f) {
			isOpened_ = true;
			isClosed_ = false;
			openT_ = 0.0f;
			isEnd = true;
		}
	}
	return isEnd;
}
