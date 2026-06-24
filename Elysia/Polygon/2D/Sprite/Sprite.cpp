#include "Sprite.h"
#include <vector>

#include "WindowsSetup.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "Matrix4x4.h"

Elysia::Sprite::Sprite() {
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
	//DirectXクラスを取得
	directXSetup_ = Elysia::DirectXSetup::GetInstance();
	//パイプライン管理クラスを取得
	pipelineManager_ = Elysia::PipelineManager::GetInstance();
}

std::unique_ptr<Elysia::Sprite>  Elysia::Sprite::Create(const uint32_t& textureHandle) {
	//生成
	std::unique_ptr<Elysia::Sprite> sprite = std::make_unique<Elysia::Sprite>();
	
	sprite->color_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };

	//テクスチャの情報を取得
	D3D12_RESOURCE_DESC resourceDesc_ = {}; resourceDesc_ = Elysia::TextureManager::GetInstance()->GetResourceDesc(textureHandle);
	//サイズを取得
	sprite->size_ = { .x = static_cast<float_t>(resourceDesc_.Width),.y = static_cast<float_t>(resourceDesc_.Height) };

	//頂点リソースを作る
	sprite->vertexResource_ = sprite->directXSetup_->CreateBufferResource(sizeof(VertexData) * 6u);
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	sprite->vertexBufferView_.BufferLocation = sprite->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	sprite->vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4u;
	//１頂点あたりのサイズ
	sprite->vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//index用のリソースを作る
	sprite->indexResource_ = sprite->directXSetup_->CreateBufferResource(sizeof(uint32_t) * 6u);
	//Indexを利用
	//リsp－スの先頭のアドレスから使う
	sprite->indexBufferView_.BufferLocation = sprite->indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	sprite->indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6u;
	//インデックスはuint32_tとする
	sprite->indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	sprite->materialResource_ = Elysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(MaterialData));
	//TransformationMatrix用のリソースを作る。
	sprite->transformationMatrixResource_ = Elysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix));


	//UVトランスフォームの初期化
	sprite->uvTransform_ = {
		//S
		{.x = 1.0f,. y = 1.0f,.z = 1.0f},
		//R
		{.x = 0.0f,.y = 0.0f,.z = 0.0f},
		//T
		{.x = 0.0f,.y = 0.0f,.z = 0.0f}
	};

	return sprite;
}

void Elysia::Sprite::Draw(const uint32_t& textureHandle){
	//非表示にするかどうか
	if (isInvisible_ ) {
		return;
	}


	//完全に透明になったら通す必要が無いよね
	if (color_.w <= 0.0f) {
		return;
	}


	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//今回は「Sprite」なので三角形が2枚必要
	//-------------
	//|\		  |
	//|  \		  |
	//|    \	  |
	//|      \    |
	//|         \ |
	//-------------

	//13
	//02




	//書き込むためのアドレスを取得
	vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData_));

	float_t left = (0.0f - anchorPoint_.x) * size_.x;
	float_t right = (1.0f - anchorPoint_.x) * size_.x;
	float_t top = (0.0f - anchorPoint_.y) * size_.y;
	float_t bottom = (1.0f - anchorPoint_.y) * size_.y;


	float_t texLeft = 0.0f;
	float_t texRight = 1.0f;
	float_t texTop = 0.0f;
	float_t texBottom = 1.0f;

	//UVをいじりたいとき設定するもの
	if (isUVSetting_) {
		//uv
		texLeft = textureLeftTop_.x / size_.x;
		texRight = (textureLeftTop_.x + textureSize_.x) / size_.x;
		texTop = textureLeftTop_.y / size_.y;
		texBottom = (textureLeftTop_.y + textureSize_.y) / size_.y;
	}




	//左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}


	//左下
	vertexData_[LeftBottom].position = { left,bottom,0.0f,1.0f };
	vertexData_[LeftBottom].texCoord = { texLeft,texBottom };
	//左上
	vertexData_[LeftTop].position = { left,top,0.0f,1.0f };
	vertexData_[LeftTop].texCoord = { texLeft,texTop };
	//右下
	vertexData_[RightBottom].position = { right,bottom,0.0f,1.0f };
	vertexData_[RightBottom].texCoord = { texRight,texBottom };
	//右上
	vertexData_[RightTop].position = { right,top,0.0f,1.0f };
	vertexData_[RightTop].texCoord = { texRight,texTop };
	vertexResource_->Unmap(0u, nullptr);


	//IndexResourceにデータを書き込む
	//インデックスデータにデータを書き込む
	indexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0u;
	indexData_[1] = 1u;
	indexData_[2] = 2u;
	indexData_[3] = 1u;
	indexData_[4] = 3u;
	indexData_[5] = 2u;
	indexResource_->Unmap(0u, nullptr);



	//トランスフォームデータに書き込み
	transformationMatrixResource_->Map(0u, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));

	//座標の再設定
	Vector3 newPosition = {};
	if (isBack_) {
		newPosition = { .x = position_.x,.y = position_.y,.z = 1.0f };
	}
	else {
		newPosition = { .x = position_.x,.y = position_.y,.z = 0.0f };
	}


	//アフィン行列を計算
	Matrix4x4 affineMatrix = Matrix4x4::MakeAffineMatrix({ scale_.x,scale_.y,1.0f }, { 0.0f,0.0f,rotate_ }, newPosition);
	//遠視投影行列を計算
	Matrix4x4 viewMatrix = Matrix4x4::MakeIdentity4x4();
	//プロジェクション行列を計算
	Matrix4x4 projectionMatrix = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, static_cast<float_t>(windowsSetup_->GetClientWidth()), static_cast<float_t>(windowsSetup_->GetClientHeight()), 0.0f, 100.0f);
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(affineMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = Matrix4x4::MakeIdentity4x4();
	transformationMatrixResource_->Unmap(0u, nullptr);


	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0u, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	//ライティングしない
	materialData_->lightingKinds = NoneLighting;
	materialData_->shininess = 0.0f;

	Matrix4x4 uvTransformMatrix = Matrix4x4::MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Matrix4x4::Multiply(uvTransformMatrix, Matrix4x4::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Matrix4x4::Multiply(uvTransformMatrix, Matrix4x4::MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;
	materialResource_->Unmap(0u, nullptr);

	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetSpriteRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetSpriteGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, materialResource_->GetGPUVirtualAddress());
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, transformationMatrixResource_->GetGPUVirtualAddress());


	if (textureHandle != 0u) {
		Elysia::TextureManager::GetInstance()->GraphicsCommand(2u, textureHandle);
	}

	//今度はこっちでドローコールをするよ
	//描画(DrawCall)6個のインデックスを使用し1つのインスタンスを描画。
	directXSetup_->GetCommandList()->DrawIndexedInstanced(6u, 1u, 0u, 0u, 0u);


}
