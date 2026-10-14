#pragma once

/**
 * @file Jacket.h
 * @brief ジャケットのクラス
 * @author 茂木翼
 */

#include <vector>
#include <string>
#include <Sprite.h>

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
	/// <param name="jacketPath"></param>
	void Initialize(const std::vector<std::string> jacketPath);

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
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;

private:
	/// <summary>
	/// ジャケットで使うアセット集
	/// </summary>
	struct Asset {
		//本体
		std::unique_ptr<Elysia::Sprite> mainSprite;
		//難易度のフレーム
		std::unique_ptr<Elysia::Sprite> levelFrameSprite;
		//難易度の色
		Vector4<float_t> levelColor;

	};

private:
	//ジャケットのアセット
	std::vector<Asset> assetVector_ = {};

	//現在選択されている難易度
	std::string

};

