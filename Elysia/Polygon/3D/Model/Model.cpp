#include "Model.h"

#include <numbers>
#include <cassert>

#include "Camera.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "PipelineManager.h"

#include "SrvManager.h"
#include "WorldTransform.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

Elysia::Model::Model() {
	//テクスチャ管理クラスの取得
	textureManager_ = Elysia::TextureManager::GetInstance();
	//モデル管理クラスの取得
	modelManager_ = Elysia::ModelManager::GetInstance();
	//DirectXクラスの取得
	directXSetup_ = Elysia::DirectXSetup::GetInstance();
	//パイプライン管理クラスの取得
	pipelineManager_ = Elysia::PipelineManager::GetInstance();
	//SRV管理クラスも取得
	srvManager_ = Elysia::SrvManager::GetInstance();
}
std::unique_ptr<Elysia::Model> Elysia::Model::Create(const uint32_t& modelHandle) {

	//生成
	std::unique_ptr<Elysia::Model> model = std::make_unique<Elysia::Model>();

	
	//モデルデータの読みこみ
	model->modelData_ = model->modelManager_->GetModelData(modelHandle);
	//テクスチャの読み込み
	model->textureHandleVector_.resize(model->modelData_.materials.size(), 0u);
	for(size_t i=0u;i<model->modelData_.materials.size();++i){
		const std::string& PATH = model->modelData_.materials[i].textureFilePath;
		if (PATH.empty()) {
			//空の場合テクスチャーにする
			model->textureHandleVector_[i] = model->textureManager_->Load("Resources/Sprite/Back/White.png");
		}
		else {
			//通常のテクスチャー
			model->textureHandleVector_[i] = model->textureManager_->Load(PATH);
		}
	}

	//頂点リソースを作る
	model->vertexResource_ = model->directXSetup_->CreateBufferResource(sizeof(VertexData) * model->modelManager_->GetModelData(modelHandle).vertices.size());
	//リソースの先頭のアドレスから使う
	model->vertexBufferView_.BufferLocation = model->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	model->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model->modelData_.vertices.size());
	//１頂点あたりのサイズ
	model->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	model->vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, model->modelData_.vertices.data(), sizeof(VertexData) * model->modelData_.vertices.size());
	model->vertexResource_->Unmap(0u, nullptr);

	
	//解析したデータを使ってResourceとBufferViewを作成する
	model->indexResource_ = model->directXSetup_->CreateBufferResource(sizeof(uint32_t) * model->modelManager_->GetModelData(modelHandle).indices.size());
	//場所
	model->indexBufferView_.BufferLocation = model->indexResource_->GetGPUVirtualAddress();
	//サイズ
	model->indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * model->modelData_.indices.size());
	//フォーマット
	model->indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックス
	uint32_t* index = nullptr;
	model->indexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&index));
	std::memcpy(index, model->modelData_.indices.data(), sizeof(uint32_t) * model->modelData_.indices.size());
	model->indexResource_->Unmap(0u, nullptr);


	//カメラ
	model->cameraResource_ = model->directXSetup_->CreateBufferResource(sizeof(CameraForGPU));

	return model;

}

void Elysia::Model::Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material){
	//非表示設定がtrueになっていた場合は描画しない
	if (isInvisible_ ) {
		return;
	}

	//ライティング無しだけ
	assert(material.lightingKinds == LightingType::NoneLighting);

	//PixelShaderに送る方のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	cameraForGPU_->worldPosition = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);

	//コマンドを積む
	//パイプラインの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetModelGraphicsPipelineState().Get());
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Material
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource->GetGPUVirtualAddress());
	//コマンド送ってGPUで計算
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, worldTransform.resource->GetGPUVirtualAddress());
	
	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4u, camera.resource->GetGPUVirtualAddress());
	//PixelShaderに送る方のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());
	//DrawCall
	//テクスチャー
	for (const MeshRange& range : modelData_.meshRanges) {
		const uint32_t textureHandle = textureHandleVector_[range.materialIndex];
		textureManager_->GraphicsCommand(2u, textureHandle);
		directXSetup_->GetCommandList()->DrawIndexedInstanced(range.indexCount, 1u, range.indexStart, 0u, 0u);
	}
}

void Elysia::Model::Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material, const BaseLight& baseLight){
	//非表示設定がtrueになっていた場合は描画しない
	if (isInvisible_) {
		return;
	}

	//PixelShaderに送る方のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	cameraForGPU_->worldPosition = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);

	//コマンドを積む
	//PSOの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetModelGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Material
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource->GetGPUVirtualAddress());
	//コマンド送ってGPUで計算
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, worldTransform.resource->GetGPUVirtualAddress());
	
	//ライトの設定
	switch (material.lightingKinds) {
	case LightingType::DirectionalLighting:
		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, baseLight.resource->GetGPUVirtualAddress());
		break;

	case LightingType::PointLighting:
		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(6u, baseLight.resource->GetGPUVirtualAddress());
		break;

	case LightingType::SpotLighting:
		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(7u, baseLight.resource->GetGPUVirtualAddress());
		break;
	}
	
	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4u, camera.resource->GetGPUVirtualAddress());
	//PixelShaderに送る方のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());
	
	//環境マッピングの設定
	if (material.isEnvironmentMap && environmentTextureHandle_ != 0u) {
		srvManager_->SetGraphicsRootDescriptorTable(8u, environmentTextureHandle_);
	}

	//テクスチャー
	for (const MeshRange& range : modelData_.meshRanges) {
		const uint32_t textureHandle = textureHandleVector_[range.materialIndex];
		textureManager_->GraphicsCommand(2u, textureHandle);
		//DrawCall
		directXSetup_->GetCommandList()->DrawIndexedInstanced(range.indexCount, 1u, range.indexStart, 0u, 0u);
	}

	
}
