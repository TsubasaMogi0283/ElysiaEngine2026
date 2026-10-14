#include "Jacket.h"

#include <TextureManager.h>
#include <Color.h>

Jacket::Jacket(){
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void Jacket::Initialize(const std::vector<uint32_t> jacketTextureHandleVector, const uint32_t& frameTextureHandle){
	
	for (const auto& h : jacketTextureHandleVector) {
		Asset asset = {};
		//テクスチャの読み込み
		//スプライトの生成
		asset.mainSprite = Elysia::Sprite::Create(h);
		asset.levelFrameSprite = Elysia::Sprite::Create(frameTextureHandle);
		//挿入
		assetVector_.push_back(asset);
	}
	
}

void Jacket::Update(){

	
	for (const auto& asset : assetVector_) {


		//難易度によって色を変える
		Vector4<float_t> frameColor = {};
		switch (currentDifficulty_) {
		case Difficulty::Easy:
			//簡単の色を設定
			frameColor = Color::Conevert(Color::EASY);
			break;

		case Difficulty::Normal:
			//普通の色を設定
			frameColor = Color::Conevert(Color::NORMAL);
			break;

		case Difficulty::Hard:
			//難しいの色を設定
			frameColor = Color::Conevert(Color::HARD);
			break;

		case Difficulty::Special:
			//特別の色を設定
			//虹でもあり?笑
			frameColor = Color::Conevert(Color::SPECIAL);
			break;
		}

		//色の変更
		asset.levelFrameSprite->SetColor(frameColor);
	}
}

void Jacket::DrawSprite(){
	for (const auto& asset : assetVector_) {
		//メイン
		asset.mainSprite->Draw();
		//フレーム
		asset.levelFrameSprite->Draw();

	}

}
