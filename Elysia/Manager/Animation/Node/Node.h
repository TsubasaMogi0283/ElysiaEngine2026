#pragma once

/**
 * @file Node.h
 * @brief ノードの構造体
 * @author 茂木翼
 */

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Matrix4x4.h>
#include <QuaternionTransform.h>

//親からどんどん子に伸びていく

/// <summary>
/// ノード
/// </summary>
struct Node {
public:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	static Node Read(aiNode* node);

public:
	//トランスフォーム
	QuaternionTransform transform = {};
	//ローカル行列
	Matrix4x4 localMatrix = {};
	//名前
	std::string name="";
	//子ノード
	std::vector<Node> children = {};
};