#include "Transition.h"
#include <Windows/WindowsSetup.h>
#include <TextureManager.h>


void Transition::Initialize(){
	//float_t windowHeight = Elysia::WindowsSetup::GetInstance()->GetClientSize().y;
	textureHandle_ = Elysia::TextureManager::GetInstance()->Load("Resources/Sprite/Back/White.png");


	uint64_t textureHeight = Elysia::TextureManager::GetInstance()->GetTextureHeight(textureHandle_);
	heightScale_ = static_cast<float_t>(DIVIDE_NUMBER_)/ static_cast<float_t>(textureHeight);
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;++i) {
		spriteArray_[i] = Elysia::Sprite::Create(textureHandle_);
		spriteArray_[i]->SetScale({ .x = 1.0f, .y = heightScale_ });
	}
}

void Transition::Update(){
}

void Transition::DrawSprite(){
	for (uint8_t i = 0u;i < DIVIDE_NUMBER_;++i) {
		spriteArray_[i]->Draw();
	}
}
