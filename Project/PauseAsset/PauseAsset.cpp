#include "PauseAsset.h"

void PauseAsset::Initilaize(const uint32_t& blackTextureHandle, const std::vector<uint32_t>& countNumberTextureHandleVector) {
	backSprite_ = Elysia::Sprite::Create(blackTextureHandle);
	backSprite_->SetTransparency(PAUSE_TRANSPARENCY_);

	for (uint8_t i = 0;i < countNumberTextureHandleVector.size();i++) {
		//数字の情報を格納
		NumberInformation numberInformation = {
			.textureHandle = countNumberTextureHandleVector[i],
			.number = i + 1,
		};
		numberInformationVector_.push_back(numberInformation);
	}
}

void PauseAsset::Update() {
	if (timer_) {
		
	}

	currentNumberTextureHandle_ = numberInformationVector_[static_cast<uint8_t>(timer_) - 1u].textureHandle;

}

void PauseAsset::Draw() {
	//黒背景
	backSprite_->Draw();
	
	//カウント
	for (uint8_t i = 0;i < numberInformationVector_.size();i++) {
		numberSprite_->Draw(currentNumberTextureHandle_);
	}
}