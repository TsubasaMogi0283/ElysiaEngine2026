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
	/// <param name="dModelhandle"></param>
	/// <param name="aModelHandle"></param>
	/// <param name="pModelHandle"></param>
	/// <param name="exclamationModelhandle"></param>
	void Initialize(const uint32_t& dModelhandle,const uint32_t& aModelHandle,const uint32_t& pModelHandle,const uint32_t& exclamationModelhandle);
	
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
	/// 各文字の情報
	/// </summary>
	struct TextObjectInformation {
		//モデル
		std::unique_ptr<Elysia::Model>model_;
		//ワールドトランスフォーム
		WorldTransform worldTransform;
		//マテリアル
		Material material;
	};

private:
	//Dの数
	static const uint8_t D_OBJECT_AMOUNT_ = 2u;
	//Aの数
	static const uint8_t A_OBJECT_AMOUNT_ = 3u;
	//!の数
	static const uint8_t EXCLAMATION_OBJECT_AMOUNT_ = 3u;


private:
	/// <summary>
	/// オブジェクトの生成
	/// </summary>
	/// <param name="objectInformation"></param>
	void GenerateTextObject(TextObjectInformation& objectInformation, const uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="objectInformation"></param>
	/// <param name="modelHandle"></param>
	void UpdateTextObject(TextObjectInformation& objectInformation, const uint32_t modelHandle);

private:
	//Dのモデル
	std::array<TextObjectInformation, D_OBJECT_AMOUNT_>dObjectArray_ = {};
	//Aのモデル
	std::array<TextObjectInformation, A_OBJECT_AMOUNT_>aObjectArray_ = {};
	//Pのモデル
	TextObjectInformation pObject_ = {};
	//!のモデル
	std::array<TextObjectInformation, EXCLAMATION_OBJECT_AMOUNT_>exclamationObjectArray_ = {};


};
