#pragma once

/**
 * @file MusicSelectScene.h
 * @brief 楽曲選択シーンクラス
 * @author 茂木翼
 */


#include <SelectScene/BaseSelectScene.h>
#include <Sprite.h>

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
	void DrawObject([[maybe_unused]] const Camera& camera, [[maybe_unused]] const BaseLight& baseLight) {};

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite()override;

private:

	/// <summary>
	/// トランジションを開ける
	/// </summary>
	void OpenTransition();

	/// <summary>
	/// 楽曲選択
	/// </summary>
	void MusicSelect();

	/// <summary>
	/// 選択
	/// </summary>
	void Decide();

private:
	/// <summary>
	/// 各状態を実行
	/// </summary>
	typedef void (MusicSelectScene::* function)();

	/// <summary>
	/// テーブル
	/// </summary>
	inline static void (MusicSelectScene::* functionTable[])() = {
		&MusicSelectScene::OpenTransition,
		& MusicSelectScene::MusicSelect,
		& MusicSelectScene::Decide,
	};

private:

	/// <summary>
	/// スタートメインシーンの状態
	/// </summary>
	enum class MusicSelectSceneState {
		//開けるトランジション
		OpenTransition,
		//楽曲選択
		MusicSelect,
		//決定
		Decide,

		//この列挙体の量
		Amount,
	};

	//現在の状態
	MusicSelectSceneState currentState_ = MusicSelectSceneState::OpenTransition;



private:


	/// <summary>
	/// 楽曲のジャケット情報
	/// </summary>
	struct MusicJacketInformation {
		//ジャケットメイン部分
		std::unique_ptr<Elysia::Sprite>mainSprite_ = nullptr;
		//ハンドル
		uint32_t mainTextureHandle = 0u;
	};

	/// <summary>
	/// 矢印
	/// </summary>
	struct Arrow {
		enum Direction {
			//左
			Left,
			//右
			Right,
		};

		//スプライト
		std::unique_ptr<Elysia::Sprite> sprite = nullptr;
		//座標
		Vector2<float_t>position = {};
	};

private:
	//矢印の数
	static const uint8_t ARRAOW_AMOUNT_ = 2u;


private:

	//矢印
	std::array<Arrow, ARRAOW_AMOUNT_> arrowArray_ = {};


	//テクスチャの読みこみ
	std::vector<uint32_t>jacketTextureHandleVector = {};

	//現在選択している楽曲の番号
	uint8_t currentSelectedMusicNumber_ = 0u;
	//現在再生中の楽曲番号
	int32_t playingMusicNumber_ = -1;

	//長押ししているかどうか
	//楽曲の高速移動で使う
	bool isLongPush_ = false;

	//効果音
	//決定
	int32_t decideSEHandle_ = -1;
	//選択
	int32_t selectSEHandle_ = -1;
};

