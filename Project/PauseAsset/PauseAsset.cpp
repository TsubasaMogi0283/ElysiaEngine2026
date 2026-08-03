#include "PauseAsset.h"
#include <imgui.h>

void PauseAsset::Initilaize(const uint32_t& blackTextureHandle, const std::vector<uint32_t>& countNumberTextureHandleVector) {
	backSprite_ = Elysia::Sprite::Create(blackTextureHandle);
	backSprite_->SetTransparency(PAUSE_TRANSPARENCY_);

	//数字のテクスチャのスプライトを作成
	numberSprite_ = Elysia::Sprite::Create(blackTextureHandle);

	for (uint8_t i = 0;i < countNumberTextureHandleVector.size();i++) {
		//数字の情報を格納
		NumberInformation numberInformation = {
			.textureHandle = countNumberTextureHandleVector[i],
			.number = i + 1u,
		};
		numberInformationVector_.push_back(numberInformation);
	}
}

void PauseAsset::Update() {




	for(size_t i = 0; i < numberInformationVector_.size(); i++) {
		if (numberInformationVector_[i].number == static_cast<uint8_t>(timer_)) {
			currentNumberTextureHandle_ = numberInformationVector_[i].textureHandle;
			break;
		}
	}

#ifdef _DEBUG
	ImGui::Begin("ポーズ機能");

	int32_t newHandle = static_cast<int32_t>(currentNumberTextureHandle_);
	ImGui::InputInt("New Texture Handle", &newHandle);
	ImGui::End();
#endif // _DEBUG


}

void PauseAsset::Draw() {
	//黒背景
	backSprite_->Draw();
	
	//カウント
	for (uint8_t i = 0;i < numberInformationVector_.size();i++) {
		numberSprite_->Draw(currentNumberTextureHandle_);
	}
}