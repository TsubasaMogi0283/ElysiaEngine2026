#pragma once

/**
 * @file BaseNote.h
 * @brief ノーツの基底クラス
 * @author 茂木翼
 */

#include <Model.h>
#include <WorldTransform.h>
#include <Material.h>


/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// ノーツの基底クラス
/// </summary>
class BaseNote{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	virtual void Initialize(const uint32_t& modelHandle) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="baseLight"></param>
	virtual void DrawObject3D(const Camera& camera,const BaseLight& baseLight) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseNote() = default;

public:
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetPosition(const Vector3& position) {
		this->worldTransform_.translate = position;
	}

	/// <summary>
	/// 使用状態を設定
	/// </summary>
	/// <param name="isUsed">使用状態</param>
	inline void SetIsUsed(const bool& isUsed) {
		this->isUsed_ = isUsed;
	}

	/// <summary>
	/// 使用状態を取得
	/// </summary>
	/// <returns>使用状態</returns>
	inline bool GetIsUsed()const {
		return isUsed_;
	}

	/// <summary>
	/// 移動の比率を設定
	/// </summary>
	/// <param name="ratio">比率</param>
	inline void SetRatio(const float_t& ratio) {
		this->ratio_ = ratio;
	}

	/// <summary>
	/// 移動の比率を取得
	/// </summary>
	/// <returns>比率</returns>
	inline float_t GetRatio()const {
		return ratio_;
	}

	/// <summary>
	/// 初期座標
	/// </summary>
	/// <param name="positionX">X座標</param>
	inline void SetInitialPositionX(const float_t& positionX) {
		this->initialPositionX_ = positionX;
	}

	/// <summary>
	/// 判定の座標を設定
	/// </summary>
	/// <param name="positionX">X座標</param>
	inline void SetJudgmentPositionX(const float_t& positionX) {
		this->judgmentPositionX_ = positionX;
	}

	/// <summary>
	/// レーンの座標を設定
	/// </summary>
	/// <param name="positionY">Y座標</param>
	inline void SetLanePositionY(const float_t& positionY) {
		this->lanePositionY_ = positionY;
	}

	/// <summary>
	/// 移動開始時間を設定
	/// </summary>
	/// <param name="time">時間</param>
	inline void SetStartMoveTime(const float_t& time) {
		this->startMoveTime_ = time;
	}

	/// <summary>
	/// 到達時間を設定
	/// </summary>
	/// <param name="time">時間</param>
	inline void SetArriveLineTime(const float_t& time) {
		this->arriveLineTime_ = time;
	}

	/// <summary>
	/// 楽曲時間の設定
	/// </summary>
	/// <param name="time">時間</param>
	inline void SetMusicTime(const float_t& time) {
		this->musicTime_ = time;
	}

	/// <summary>
	/// オブジェクトプール用の割り当て番号を設定
	/// </summary>
	/// <param name="index">割り当て番号</param>
	inline void SetPoolIndex(const uint32_t& index) {
		this->poolIndex_ = static_cast<int8_t>(index);
	}

	/// <summary>
	/// オブジェクトプール用の割り当て番号を取得
	/// </summary>
	/// <returns>割り当て番号</returns>
	inline int8_t GetPoolIndex()const {
		return poolIndex_;
	}

#ifdef _DEBUG
	
	/// <summary>
	/// 移動開始時間の取得
	/// </summary>
	/// <returns></returns>
	inline float_t GetStartMoveTime()const {
		return startMoveTime_;
	}

	/// <summary>
	/// 到達時間の取得
	/// </summary>
	/// <returns></returns>
	inline float_t GetArriveLineTime()const {
		return arriveLineTime_;
	}

	/// <summary>
	/// Y座標の取得
	/// </summary>
	/// <returns></returns>
	inline float_t GetJudgmentPositionY()const {
		return lanePositionY_;
	}

#endif // _DEBUG


protected:
	//モデル
	std::unique_ptr<Elysia::Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
	//使用状態
	bool isUsed_ = false;
	//動きの比率
	float_t ratio_ = 0.0f;

	//初期座標
	float_t initialPositionX_ = 0.0f;
	//判定座標
	float_t judgmentPositionX_ = 0.0f;
	//レーンの座標
	float_t lanePositionY_ = 0.0f;

	//移動開始時間
	float_t startMoveTime_ = 0.0f;
	//到達時間
	float_t arriveLineTime_ = 0.0f;
	//楽曲時間
	float_t musicTime_ = 0.0f;
	//オブジェクトプールの割り当て番号
	int8_t poolIndex_ = 0u;
};

