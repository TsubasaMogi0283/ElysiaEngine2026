#include "Node.h"

#include <Vector3.h>

Node Node::Read(aiNode* node) {
	Node result = {};

	aiVector3D scale = {};
	aiVector3D translate = {};
	aiQuaternion rotate = {};
	//assimpの行列からSRTを抽出する関数を利用
	node->mTransformation.Decompose(scale, rotate, translate);
	//Scale
	result.transform.scale = { .x = scale.x, .y = scale.y, .z = scale.z };
	//rotate
	result.transform.rotate = { .x = rotate.x, .y = -rotate.y, .z = -rotate.z, .w = rotate.w };
	//translate
	result.transform.translate = { .x = -translate.x, .y = translate.y, .z = translate.z };

	Vector3<float_t> newRotate = { .x = result.transform.rotate.x, .y = result.transform.rotate.y, .z = result.transform.rotate.z };
	result.localMatrix = Matrix4x4::MakeAffineMatrix(result.transform.scale, newRotate, result.transform.translate);


	//Node名を格納
	result.name = node->mName.C_Str();
	//子の数だけ確保
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = Read(node->mChildren[childIndex]);
	}

	return result;
}
