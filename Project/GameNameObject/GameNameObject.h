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
	void Initialize();
	
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

private:
	/// <summary>
	/// 各文字の
	/// </summary>
	struct TextObject {
		//モデル
		std::unique_ptr<Elysia::Model>model_;
		//ワールドトランスフォーム
		WorldTransform worldTransform;
		//マテリアル
		Material material;
	};



private:
	//Dのモデル
	std::array<std::unique_ptr<Elysia::Model>, 2u>dModelArray_ = {};
	//Aのモデル
	std::array<std::unique_ptr<Elysia::Model>, 3u>aModelArray_ = {};
	//Pのモデル
	std::unique_ptr<Elysia::Model>pModel_ = nullptr;
	//!のモデル
	std::array<std::unique_ptr<Elysia::Model>, 3u>exclamationMarkModelArray_ = {};


};
