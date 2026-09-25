#include "Transition.h"
#include <Windows/WindowsSetup.h>
#include <TextureManager.h>
#include <Easing.h>


void Transition::Initialize(){
	//テクスチャの読み込み
	textureHandle_ = Elysia::TextureManager::GetInstance()->Load("Resources/Sprite/Back/White.png");

	//テクスチャの縦幅を取得
	uint64_t textureHeight = Elysia::TextureManager::GetInstance()->GetTextureHeight(textureHandle_);
	heightScale_ = 1.0f / static_cast<float_t>(DIVIDE_NUMBER_);
	//スプライトの生成
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;i++) {
		spriteArray_[i] = Elysia::Sprite::Create(textureHandle_);
		spriteArray_[i]->SetScale({ .x = widthScale_, .y = heightScale_ });
		//偶数列の時は右から
		float_t anchorPointX = 1.0f;
		int32_t positionX = 0;
		if (i % 2u == 0u) {
			anchorPointX = 1.0f;
			positionX = static_cast<int32_t>(Elysia::WindowsSetup::GetInstance()->GetClientSize().x);
		}
		else {
			anchorPointX = 0.0f;
			positionX = 0;
		}
		spriteArray_[i]->SetAnchorPoint({ .x = anchorPointX, .y = 0.0f });
		spriteArray_[i]->SetPosition({ .x = positionX, .y = static_cast<int32_t>(i * textureHeight / DIVIDE_NUMBER_) });

	}
}

void Transition::Update(){

	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;i++) {
		spriteArray_[i]->SetScale({ .x = widthScale_, .y = heightScale_ });
	}

#ifdef _DEBUG
	ImGui::Begin("トランジション");
	ImGui::SliderFloat("幅スケール", &widthScale_, 0.0f, 1.0f);
	ImGui::End();
#endif // _DEBUG


}

void Transition::DrawSprite(){
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;i++) {
		spriteArray_[i]->Draw();
	}
}

bool Transition::SetOpenTransition(){

	bool isEnd = false;

	//閉じている状態の時に開く
	if (!isOpened_&& isClosed_) {
		
		//イージングで滑らかに開ける
		openT_ += DELTA_TIME_;
		//最大値になったら開いた状態にする
		if (openT_ >= 1.0f) {
			isOpened_ = true;
			isClosed_ = false;
			openT_ = 1.0f;
			isEnd = true;
		}
		widthScale_ = 1.0f - Easing::EaseInOutQuad(openT_);
		widthScale_ = std::clamp(widthScale_, 0.0f, 1.0f);

	}
	return isEnd;
}
