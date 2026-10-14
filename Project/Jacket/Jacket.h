#pragma once

/**
 * @file Jacket.h
 * @brief ジャケットのクラス
 * @author 茂木翼
 */

#include <vector>
#include <string>
#include <Sprite.h>
#include <ScoreData/Difficulty.h>

 /// <summary>
 /// ElysiaEngine(前方宣言)
 /// </summary>
namespace Elysia {
	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;
}

/// <summary>
/// ジャケット(選曲用)
/// </summary>
class Jacket{
public:
	/// <summary>
	/// ジャケット
	/// </summary>
	Jacket();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="jacketTextureHandleVector"></param>
	/// <param name="frameTextureHandle"></param>
	void Initialize(const std::vector<uint32_t> jacketTextureHandleVector, const uint32_t& frameTextureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Jacket() = default;

private:
	/// <summary>
	/// ジャケットで使うアセット集
	/// </summary>
	struct Asset {
		//本体
		std::unique_ptr<Elysia::Sprite> mainSprite;
		//難易度のフレーム
		std::unique_ptr<Elysia::Sprite> levelFrameSprite;

		//選択されているか
		bool isSelect_ = false;
		//場所の設定
		uint8_t placeNumber;

		//座標
		Vector2<int32_t> position;
		//移動が終わったかどうか
		bool isMoved_ = false;
	};

public:
	/// <summary>
	/// 右に移動する設定
	/// </summary>
	/// <param name="isMove">移動するか</param>
	inline void SetIsMoveToRight(const bool& isMove) {

		//ジャケット情報が入って初期化されていないと使えないようにする
		assert(isInitialized_);
		if (currentSelectNumber_ < assetVector_.size()) {
			//選択している番号を増やす
			this->currentSelectNumber_++;
			//移動する
			this->isMoveToRight_ = isMove;
		}
	}

	/// <summary>
	/// 左に移動する設定
	/// </summary>
	/// <param name="isMove">移動するか</param>
	inline void SetIsMoveToLeft(const bool& isMove) {

		//ジャケット情報が入って初期化されていないと使えないようにする
		assert(isInitialized_);
		if (currentSelectNumber_ > 0u) {
			//選択している番号を減らす
			this->currentSelectNumber_--;
			//移動する
			this->isMoveToLeft_ = isMove;
		}
	}

	/// <summary>
	/// 決定の設定
	/// </summary>
	/// <param name="isDecide">決定</param>
	inline void SetIsDecide(const bool& isDecide) {
		this->isDecide_ = isDecide;
	}

	/// <summary>
	/// 終了状態を取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsProcessEnd()const {
		return isProcessEnd_;
	}

private:

	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//ウィンドウ管理クラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;

private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;

	//ジャケットの隣同士の間隔
	const int32_t JACKET_INTERVAL_X_ = 100;
	//選択時のスケール
	const float_t SELECTED_SCALE_ = 0.5f;
	//非選択時のスケール
	const float_t NO_SELECTED_SCALE_ = SELECTED_SCALE_ / 2.0f;
	//縮小時間
	const float_t SCALE_DOWN_TIME_ = 0.25f;
private:
	//ジャケットのアセット
	std::vector<Asset> assetVector_ = {};

	//現在選択されている難易度
	Difficulty currentDifficulty_ = Difficulty::Normal;
	uint8_t currentSelectNumber_ = 0u;

	//選択されているときの座標
	Vector2<int32_t> selectedPosition_ = {};

	//右に動いたかどうか
	bool isMoveToRight_ = false;
	//左に動いたかどうか
	bool isMoveToLeft_ = false;

	//決定されたかどうか
	bool isDecide_ = false;
	//縮小時間
	float_t scaleDownTime_ = 0.0f;

	//初期化済みかどうか
	bool isInitialized_ = false;
	//処理終了
	bool isProcessEnd_ = false;

};

