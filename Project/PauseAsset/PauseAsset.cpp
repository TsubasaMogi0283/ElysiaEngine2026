#include "PauseAsset.h"

void PauseAsset::Initilaize(const uint32_t& blackTextureHandle, const std::vector<uint32_t>& countNumberTextureHandles) {
	backSprite_ = Elysia::Sprite::Create(blackTextureHandle);
	backSprite_->SetTransparency(PAUSE_TRANSPARENCY_);

	for (uint8_t i = 0;i < countNumberTextureHandles.size();i++) {
		countSpriteVector_[i]=Elysia::Sprite::Create(countNumberTextureHandles[i]);
	}
}

void PauseAsset::Update() {

}

void PauseAsset::Draw() {
	//黒背景
	backSprite_->Draw();
	
	//カウント
	for (uint8_t i = 0;i < countSpriteVector_.size();i++) {
		countSpriteVector_[i]->Draw();
	}
}