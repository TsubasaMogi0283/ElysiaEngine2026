#include "PauseAsset.h"
#include <WindowsSetup.h>
#include <imgui.h>
#include <numbers>
#include <Easing.h>

void PauseAsset::Initilaize(const uint32_t& blackTextureHandle, const std::vector<uint32_t>& countNumberTextureHandleVector) {
	//黒背景のスプライトを生成
	backSprite_ = Elysia::Sprite::Create(blackTextureHandle);
	backSprite_->SetTransparency(PAUSE_TRANSPARENCY_);
	
	//数字のテクスチャのスプライトを生成
	numberSprite_ = Elysia::Sprite::Create(blackTextureHandle);
	//アンカーポイントの設定
	numberSprite_->SetAnchorPoint(anchorPoint_);
	//座標の設定
	numberSprite_->SetPosition({ 
		.x = static_cast<float_t>(Elysia::WindowsSetup::GetInstance()->GetClientWidth() * 0.5f),
		.y = static_cast<float_t>(Elysia::WindowsSetup::GetInstance()->GetClientHeight() * 0.5f )
	});

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
		timer_ -= DELTA_TIME_;
		//透明度の設定
		transparencyT = (SingleCalculation::InverseLerp<float_t>(TRANSPARENCY_START_TIME_,0.0f ,timer_));
		transparencyT = std::clamp(transparencyT, 0.0f, 1.0f);
		backSprite_->SetTransparency(PAUSE_TRANSPARENCY_ * (1.0f - transparencyT));

		//スケールの設定
		float_t t = SingleCalculation::Lerp(0.0f, 1.0f, std::fmod(timer_, 1.0f));
		float_t scaleT = Easing::EaseInBack(t);
		scaleT = std::clamp(scaleT, 0.0f, 1.0f);
		numberSprite_->SetScale({ .x = (1.0f - scaleT) * COUNT_DOWN_OFFSET_SCALE_SIZE_,.y = (1.0f - scaleT) * COUNT_DOWN_OFFSET_SCALE_SIZE_ });
		//回転の設定

		float_t rotateT = Easing::EaseInBack(t);
		numberSprite_->SetRotate(rotateT*std::numbers::pi_v<float_t>*1.0f);

		//テクスチャハンドルの設定
		for (size_t i = 0u; i < numberInformationVector_.size(); i++) {
			if (numberInformationVector_[i].number == static_cast<uint8_t>(timer_) + 1u) {
				currentNumberTextureHandle_ = numberInformationVector_[i].textureHandle;
				break;
			}
		}
		//0になったら再開
		if (timer_ <= 0.0f) {
			isCountDown_ = false;
			isEnd_ = true;
		}
	}
	else {
		//透明度の初期化
		backSprite_->SetTransparency(PAUSE_TRANSPARENCY_);
		//ポーズ時間を固定
		timer_ = COUNT_DOWN_TIME_;
	}
	

#ifdef _DEBUG
	ImGui::Begin("ポーズ機能");
	int32_t newTimer = static_cast<int32_t>(timer_);
	ImGui::InputFloat("透明度の線形保管", &transparencyT);
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