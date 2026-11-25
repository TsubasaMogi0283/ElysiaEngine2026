#pragma once

/**
 * @file MusicSelectScene.h
 * @brief 楽曲選択シーンクラス
 * @author 茂木翼
 */


#include <SelectScene/BaseSelectScene.h>

/// <summary>
/// 楽曲選択シーン
/// </summary>
class MusicSelectScene :public BaseSelectScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MusicSelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 3Dオブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject([[maybe_unused]] const Camera& camera, [[maybe_unused]] const BaseLight& baseLight);

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// 楽曲のジャケット情報
	/// </summary>
	struct MusicJacketInformation {
		//ジャケットメイン部分
		std::unique_ptr<Elysia::Sprite>mainSprite_ = nullptr;
		//ハンドル
		uint32_t mainTextureHandle = 0u;
	};


private:
	//トランジションの
	bool isOpenTransition_ = false;

	//テクスチャの読みこみ
	std::vector<MusicJacketInformation>musicJacketInformation_ = {};
};

