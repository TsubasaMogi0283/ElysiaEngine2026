#include "Jacket.h"

#include <TextureManager.h>

Jacket::Jacket(){
	//テクスチャ
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void Jacket::Initialize(const std::vector<std::string> jacketPath){
	
	for (const auto& h : jacketPath) {
		Asset asset = {};
		//テクスチャの読み込み
		uint32_t jacketTextureHandle = textureManager_->Load(h);
		//スプライトの生成
		asset.mainSprite = Elysia::Sprite::Create(jacketTextureHandle);
		//挿入
		assetVector_.push_back(asset);
	}
	
}

void Jacket::Update(){


}

void Jacket::DrawSprite()
{
}
