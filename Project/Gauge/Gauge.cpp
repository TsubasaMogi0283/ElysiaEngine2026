#include "Gauge.h"

#include <imgui.h>
#include <WindowsSetup.h>
#include <TextureManager.h>
#include <Note/NoteJudgement.h>

Gauge::Gauge(){
	//インスタンスの取得
	//ウィンドウ管理クラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void Gauge::Initialize(const uint32_t& mainTextureHandle, const uint32_t& frameTextureHandle){
	//スプライトの生成
	back_ = Elysia::Sprite::Create(mainTextureHandle);
	main_ = Elysia::Sprite::Create(mainTextureHandle);
	frame_ = Elysia::Sprite::Create(frameTextureHandle);
	//画像サイズ
	Vector2<uint64_t> textureSize = {
		.x = textureManager_->GetTextureWidth(mainTextureHandle),
		.y = textureManager_->GetTextureHeight(mainTextureHandle)
	};
	
	//通常表示座標
	gaugePosition_ = {
		.x = static_cast<int32_t>(windowsSetup_->GetClientSize().x/2u) - static_cast<int32_t>(textureSize.x / 2u),
		.y = windowsSetup_->GetClientSize().y - static_cast<int32_t>(textureSize.y)
	};

	//初期座標
	initialPosition_ = {
		.x = gaugePosition_.x,
		.y = gaugePosition_.y + static_cast<int32_t>(textureSize.y)
	};
	back_->SetPosition(initialPosition_);
	main_->SetPosition(initialPosition_);
	frame_->SetPosition(initialPosition_);

	//背景は灰色にする
	back_->SetColor({ .x = 0.2f,.y = 0.2f,.z = 0.2f,.w = 1.0f });
}

void Gauge::Update(){
	//最大値と最小値の範囲内にする
	currentValue_ = std::clamp(currentValue_, MIN_VALUE_, MAX_VALUE_);

	
	
	if (scale_ < 0.7f) {
		mainColor_ = { .x = 1.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	}
	else if (scale_ <1.0f) {
		mainColor_ = { .x = 1.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	} 
	else {
		mainColor_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
	}

#ifdef _DEBUG
	ImGui::Begin("ゲージ");
	ImGui::SliderFloat("スケール", &scale_,0.0f,1.0f);
	ImGui::End();
#endif // DEBUG

	main_->SetScale({ .x = scale_, .y = 1.0f });
	main_->SetColor(mainColor_);

}

void Gauge::DrawSprite(){
	//背景
	back_->Draw();
	//メイン部分
	main_->Draw();
	//フレーム
	frame_->Draw();
}

void Gauge::SetIncreaseValue(const int32_t& result){

	switch (result){
	case NoteJudgement::Selection::Perfect:
		currentValue_ += 4u;
		break;

	case NoteJudgement::Selection::Great:
		currentValue_ += 2u;
		break;
	case NoteJudgement::Selection::Good:
		currentValue_++;
		break;

	case NoteJudgement::Selection::Miss:
		currentValue_ -= 3u;
		break;
	}

}
