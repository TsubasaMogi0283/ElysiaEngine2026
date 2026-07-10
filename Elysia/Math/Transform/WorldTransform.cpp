#include "WorldTransform.h"
#include "Matrix4x4.h"

#include "DirectXSetup.h"
#include "Camera.h"

void WorldTransform::Initialize() {
	//リソースの作成
	resource = Elysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期値
	//スケール
	scale = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	//回転
	rotate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	//座標
	translate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	//アンカーポイント
	anchorPoint = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
}

void WorldTransform::Update() {

	//アンカーポイント用の行列を作る
	Vector3 anchorPointInverse = { .x = -anchorPoint.x,.y = -anchorPoint.y,.z = -anchorPoint.z };
	Matrix4x4 anchorMatrix = Matrix4x4::MakeTranslateMatrix(anchorPointInverse);
	Matrix4x4 postTranslateMatrix = Matrix4x4::MakeTranslateMatrix(anchorPoint);
	//クォータニオンを使う場合
	if (isUseQuaternion_) {
		
		//Scale
		Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(scale);
		//Rotate。行列へ変換
		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(quaternion_);
		//Translate
		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translate);
		

		worldMatrix = anchorMatrix;
		worldMatrix = Matrix4x4::Multiply(worldMatrix, scaleMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, rotateMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, postTranslateMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, translateMatrix);

	}
	//使わない場合
	else {
		//Scale
		Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(scale);
		//Rotate。行列へ変換
		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(rotate.x, rotate.y, rotate.z);
		//Translate
		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translate);

		worldMatrix = anchorMatrix;
		worldMatrix = Matrix4x4::Multiply(worldMatrix, scaleMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, rotateMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, postTranslateMatrix);
		worldMatrix = Matrix4x4::Multiply(worldMatrix, translateMatrix);


	}

	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Matrix4x4::Inverse(worldMatrix);
	
	//転置にした
	worldInverseTransposeMatrix = Matrix4x4::MakeTransposeMatrix(worldInverseMatrix);

	//転送
	Transfer();
}


void WorldTransform::Transfer() {

	//書き込み書き込む
	resource->Map(0u, nullptr, reinterpret_cast<void**>(&transformationData));
	//ワールド
	transformationData->world = worldMatrix;
	//ノーマル
	transformationData->normal = Matrix4x4::MakeIdentity4x4();
	//ワールド逆転置
	transformationData->worldInverseTranspose = worldInverseTransposeMatrix;
	//書き込み終了
	resource->Unmap(0u, nullptr);
}
