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

	//カウントダウン開始したかどうか
	if (isCountDown_) {
		//時間を減らす
		timer_--;
		//テクスチャハンドルの設定
		for (size_t i = 0u; i < numberInformationVector_.size(); i++) {
			if (numberInformationVector_[i].number == timer_/60u+1u) {
				currentNumberTextureHandle_ = numberInformationVector_[i].textureHandle;
				break;
			}
		}
		//0になったら再開
		if (timer_ <= 0u) {
			isCountDown_ = false;
			isEnd_ = true;
		}

	}
	else {
		//ポーズ時間を固定
		timer_ = PAUSE_TIME_;
	}
	

#ifdef _DEBUG
	ImGui::Begin("ポーズ機能");

	int32_t newHandle = static_cast<int32_t>(currentNumberTextureHandle_);
	int32_t newTimer = static_cast<int32_t>(timer_);
	ImGui::InputInt("New Texture Handle", &newHandle);
	ImGui::InputInt("時間", &newTimer);
	ImGui::End();
#endif // _DEBUG


}

void PauseAsset::Draw() {
	//黒背景
	backSprite_->Draw();
	
	//再開のときだけ描画
	if (isCountDown_) {
		//カウント
		numberSprite_->Draw(currentNumberTextureHandle_);
	}
	
}