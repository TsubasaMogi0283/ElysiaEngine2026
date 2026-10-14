#include "Jacket.h"

#include <algorithm>
#include <TextureManager.h>
#include <WindowsSetup.h>
#include <Color.h>
#include <SingleCalculation.h>
#include <Easing.h>

Jacket::Jacket(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void Jacket::Initialize(const std::vector<uint32_t> jacketTextureHandleVector, const uint32_t& frameTextureHandle){
	
	//アセットの初期設定
	for (size_t i = 0u;i < jacketTextureHandleVector.size();i++) {
		Asset asset = {};
		//スプライトの生成
		asset.mainSprite = Elysia::Sprite::Create(jacketTextureHandleVector[i]);
		asset.levelFrameSprite = Elysia::Sprite::Create(frameTextureHandle);

		//アンカーポイントの設定
		asset.mainSprite->SetAnchorPoint({ .x = 0.5f,.y = -1.0f });
		asset.levelFrameSprite->SetAnchorPoint({ .x = 0.5f,.y = -1.0f });

		//座標のの設定
		asset.position = { .x = static_cast<int32_t>(i) * JACKET_INTERVAL_X_ ,.y = JACKET_INITIAL_POSITION_Y_ };
		asset.mainSprite->SetPosition(asset.position);
		asset.levelFrameSprite->SetPosition(asset.position);

		//場所の番号
		asset.placeNumber = static_cast<uint8_t>(i);

		//挿入
		assetVector_.push_back(std::move(asset));
	}

	//初期化済みに設定
	isInitialized_ = true;
}

void Jacket::Update(){

	//選択時
	if (!isDecide_) {
		for (Asset& asset : assetVector_) {

			//同時押しさせないようにするためelse if使っています
			//右へ移動したとき
			if (isMoveToRight_) {
				
				//座標
				asset.position.x -= JACKET_INTERVAL_X_;
				//移動終了
				asset.isMoved_ = true;
			}
			//左へ移動したとき
			else if (isMoveToLeft_) {
				//選択している番号を増やす
				currentSelectNumber_--;
				//座標
				asset.position.x += JACKET_INTERVAL_X_;
				//移動終了
				asset.isMoved_ = true;
			}

			
			//選択中は大きくする
			if (asset.placeNumber == currentSelectNumber_) {
				asset.isSelect_ = true;

				//スケールの設定
				asset.mainSprite->SetScale({ .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ });
				asset.levelFrameSprite->SetScale({ .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ });
			}
			//選択されていない場合は小さくする
			else {
				asset.isSelect_ = false;

				//スケールの設定
				asset.mainSprite->SetScale({ .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ });
				asset.levelFrameSprite->SetScale({ .x = SELECTED_SCALE_ ,.y = SELECTED_SCALE_ });
			}

			//座標を設定
			asset.mainSprite->SetPosition(asset.position);
			asset.levelFrameSprite->SetPosition(asset.position);

			//難易度によって色を変える
			Vector4<float_t> frameColor = {};
			switch (selectedDifficulty_) {
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

		//全て移動終了したら
		bool isFinish = std::all_of(assetVector_.begin(), assetVector_.end(), [](const Asset& asset) {
			return asset.isMoved_;
			}
		);
		if (isFinish) {
			if (isMoveToRight_) {
				isMoveToRight_ = false;
			}
			if (isMoveToLeft_) {
				isMoveToLeft_ = false;
			}
			//また元に戻す
			for (Asset& asset : assetVector_) {
				asset.isMoved_ = false;
			}
		}
	}
	//決定
	else {
		//時間の加算
		scaleDownTime_ += DELTA_TIME_;
		
		for (const Asset& asset : assetVector_) {
			//選択されていないものは縮小していく
			if (!asset.isSelect_) {
				float_t t = SingleCalculation::InverseLerp(0.0f, SCALE_DOWN_TIME_, scaleDownTime_);
				t = std::clamp(t, 0.0f, 1.0f);
				float_t eased = 1.0f-Easing::EaseOutQuart(t);
				asset.mainSprite->SetScale({ .x = eased * NO_SELECTED_SCALE_,.y = eased * NO_SELECTED_SCALE_ });
			}
			
		}

		//指定時間を超えたら終了
		if (scaleDownTime_ >= SCALE_DOWN_TIME_) {
			isProcessEnd_ = true;
		}
	}
}

void Jacket::DrawSprite(){
	for (const Asset& asset : assetVector_) {
		//メイン
		asset.mainSprite->Draw();
		//フレーム
		asset.levelFrameSprite->Draw();

	}

}
