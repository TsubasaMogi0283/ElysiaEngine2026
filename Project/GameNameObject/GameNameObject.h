#pragma once

/**
 * @file GameNameObject.h
 * @brief ゲームの名前のオブジェクトクラス
 * @author 茂木翼
 */


#include <Model.h>
#include <WorldTransform.h>
#include <Material.h>
#include <memory>
#include <array>
#include <numbers>

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// ライト
/// </summary>
struct BaseLight;

/// <summary>
/// ゲームの名前のオブジェクト
/// </summary>
class GameNameObject{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameNameObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dModelHandle"></param>
	/// <param name="aModelHandle"></param>
	/// <param name="pModelHandle"></param>
	/// <param name="exclamationModelHandle"></param>
	void Initialize(const uint32_t& dModelHandle,const uint32_t& aModelHandle,const uint32_t& pModelHandle,const uint32_t& exclamationModelHandle);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dモデルの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameNameObject() = default;

public:
	/// <summary>
	/// 動き始めるかどうかを設定
	/// </summary>
	/// <param name="isMove">動き</param>
	inline void SetIsMove(const bool& isMove) {
		this->isMove_ = isMove;
	}

	/// <summary>
	/// 処理終了か取得
	/// </summary>
	inline bool GetIsProcessEnd()const {
		return isProcessEnd_;
	}


private:
	/// <summary>
	/// 単語の数
	/// </summary>
	enum class WordNumber {
		//1つ目
		First,
		//2つ目
		Second,
		//3つ目
		Third,

		//サイズ
		Size,
	};

private:
	//Dの数
	static const uint8_t D_OBJECT_AMOUNT_ = 2u;
	//Aの数
	static const uint8_t A_OBJECT_AMOUNT_ = 3u;
	//!の数
	static const uint8_t EXCLAMATION_OBJECT_AMOUNT_ = 3u;

	//単語(例:DA!で1つ)の数
	static const uint8_t WORD_AMOUNT_ = 3u;
	//文字数(例:D,A,!で3つ)
	static const uint8_t TEXT_AMOUNT_ = 3u;


	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//待機時間
	const float_t INTERVAL_TIME_ = 1.0f;
	//文字のオブジェクトが動く時間
	const float_t TEXT_OBJECT_MOVE_TIME_ = 1.0f;

	//初期の回転
	const float_t INITIAL_ROTATE_Y_ = std::numbers::pi_v<float_t>*10.0f;
	//初期のZ座標
	const float_t INITIAL_POSITION_Z_ = -10.0f;

	//最終の回転
	const float_t FINALE_ROTATE_Y_ = 0.0f;
	//最終の座標
	const float_t FINALE_POSITION_Z_ = 0.0f;

	//オブジェクトのの大きさ
	const float_t OBJECT_SCALE_ = 3.0f;

private:

	/// <summary>
	/// 1単語の情報
	/// </summary>
	struct WordInformation {

		/// <summary>
		/// 各文字の情報
		/// </summary>
		struct IndivisualTextObject {
			//モデル
			std::unique_ptr<Elysia::Model>model;
			//ワールドトランスフォーム
			WorldTransform worldTransform;
			//マテリアル
			Material material;
			//文字
			std::string text;
		};
		//ひとまとまり
		std::array<IndivisualTextObject, TEXT_AMOUNT_>objectArray;

		//動く時間
		float_t moveTime;
		//動き始める時間
		float_t startMoveTime;
		//止まる時間
		float_t endMoveTime;

		//単語の番号
		WordNumber number;
	};

private:
	
	//文字の動く時間
	std::array<WordInformation, WORD_AMOUNT_>wordArray_ = {};
	
	//動き始めるかどうか
	bool isMove_ = true;
	//処理終了かどうか
	bool isProcessEnd_ = true;

};
