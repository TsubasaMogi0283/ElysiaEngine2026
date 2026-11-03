#pragma once
/**
 * @file ModelData.h
 * @brief モデルデータの構造体
 * @author 茂木翼
 */

#include <vector>
#include <VertexData.h>
#include <Node.h>
#include <map>
#include "JoinWeightData.h"

 /// <summary>
 /// メッシュ1つ分の描画範囲
 /// </summary>
struct MeshRange {
	//indices内の開始位置
	uint32_t indexStart = 0;
	//indexの数
	uint32_t indexCount = 0;
	//materials内の番号
	uint32_t materialIndex = 0;
};



/// <summary>
/// モデルデータ
/// </summary>
struct ModelData {

	/// <summary>
	/// マテリアル情報
	/// </summary>
	struct MaterialInformation {
		std::string textureFilePath;
	};

	//スキンクラスターデータ
	std::map<std::string, JointWeightData> skinClusterData;
	//頂点
	std::vector<VertexData> vertices;
	//index
	std::vector <uint32_t>indices;
	//メッシュの描画範囲
	std::vector<MeshRange> meshRanges;
	//マテリアル情報
	std::vector<MaterialInformation> materials;
	//ノード
	Node rootNode;
};