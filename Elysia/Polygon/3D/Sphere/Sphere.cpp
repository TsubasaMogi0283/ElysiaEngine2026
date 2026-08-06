#include "Sphere.h"

#include <d3dx12.h>
#include <vector>
#include <numbers>

#include "WindowsSetup.h"


Elysia::Sphere::Sphere(){
	//インスタンスの取得
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
	//DirectX
	directXSetup_ = Elysia::DirectXSetup::GetInstance();
}

//初期化
void Elysia::Sphere::Initialize() {
	

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResource_ = directXSetup_->CreateBufferResource(sizeof(VertexData) * SUBDIVISION_ *SUBDIVISION_ * 6);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_= directXSetup_->CreateBufferResource(sizeof(MaterialData));

	//Lighting
	directionalLightResource_ = directXSetup_->CreateBufferResource(sizeof(DirectionalLightData));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	//Sphere用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResource_ = directXSetup_->CreateBufferResource(sizeof(TransformationMatrix));



	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * SUBDIVISION_ * SUBDIVISION_ * 6;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);




}





//描画
//左上、右上、左下、右下
void Elysia::Sphere::Draw(SphereShape sphereCondtion, Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ,Vector4 color) {

	
	//書き込み用のアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//経度分割1つ分の角度φd
	//円周率使いたいとき「std::numbers::pi」こうするらしい
	//<numbers>もインクルードしなければならない。めんどいね。
	//しかもdouble型。floatにしないとダメだね。腹立つね。
	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(SUBDIVISION_);

	//緯度分割1つ分の角度θd
	const float LAT_EVERY = float(std::numbers::pi)/float(SUBDIVISION_);


	//緯度の方向に分割
	for (int latIndex = 0; latIndex < SUBDIVISION_; ++latIndex) {
		//θ
		float lat = - float (std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;
		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < SUBDIVISION_; ++lonIndex) {
			uint32_t start = (latIndex * SUBDIVISION_ + lonIndex) * 6;
			
			//texcoord専用のxy座標
			float u = float(lonIndex) / float(SUBDIVISION_);
			//下から0上が1になっていたので「1.0f-～」にして逆にする
			float v = 1.0f - float(latIndex) / float(SUBDIVISION_);

			//φ
			float lon = lonIndex * LON_EVERY;
			//頂点にデータを入力する。基準点a
			//書き込む数は6つだよ
			 
			//点間の距離
			float length = 1.0f / SUBDIVISION_;

#pragma region 三角形１枚目 
			//左上(点B)が原点
			//abc
			//資料通りだとここは点a(左下)
			vertexData_[start].position.x = sphereCondtion.radius*(cos(lat) * cos(lon));
			vertexData_[start].position.y = sphereCondtion.radius*(sin(lat));
			vertexData_[start].position.z = sphereCondtion.radius*(cos(lat) * sin(lon)) ;
			vertexData_[start].position.w = 1.0f;
			//分割分移動
			vertexData_[start].texCoord.x = u;
			vertexData_[start].texCoord.y = v + length;
			//法線情報を追加
			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;

			//普通にstart+1していけばいいのかな
			//点b(左上)
			vertexData_[start + 1].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * cos(lon);
			vertexData_[start + 1].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY)) ;
			vertexData_[start + 1].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * sin(lon);
			vertexData_[start + 1].position.w = 1.0f;
			vertexData_[start + 1].texCoord.x = u;
			vertexData_[start + 1].texCoord.y = v;
			vertexData_[start + 1].normal.x = vertexData_[start + 1].position.x;
			vertexData_[start + 1].normal.y = vertexData_[start + 1].position.y;
			vertexData_[start + 1].normal.z = vertexData_[start + 1].position.z;


			//点c(右下)
			vertexData_[start + 2].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY)) ;
			vertexData_[start + 2].position.y = sphereCondtion.radius*(sin(lat));
			vertexData_[start + 2].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexData_[start + 2].position.w = 1.0f;
			vertexData_[start + 2].texCoord.x = u + length;
			vertexData_[start + 2].texCoord.y = v + length;
			vertexData_[start + 2].normal.x = vertexData_[start + 2].position.x;
			vertexData_[start + 2].normal.y = vertexData_[start + 2].position.y;
			vertexData_[start + 2].normal.z = vertexData_[start + 2].position.z;

#pragma endregion

#pragma region 三角形２枚目
			//bcd
			//点d(右上)
			vertexData_[start + 3].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon + LON_EVERY));
			vertexData_[start + 3].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexData_[start + 3].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon + LON_EVERY));
			vertexData_[start + 3].position.w = 1.0f;
			vertexData_[start + 3].texCoord.x = u + length;
			vertexData_[start + 3].texCoord.y = v;
			vertexData_[start + 3].normal.x = vertexData_[start + 3].position.x;
			vertexData_[start + 3].normal.y = vertexData_[start + 3].position.y;
			vertexData_[start + 3].normal.z = vertexData_[start + 3].position.z;


			//点c(右下)
			vertexData_[start + 4].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY));
			vertexData_[start + 4].position.y = sphereCondtion.radius*(sin(lat));
			vertexData_[start + 4].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexData_[start + 4].position.w = 1.0f;
			vertexData_[start + 4].texCoord.x = u + length;
			vertexData_[start + 4].texCoord.y = v + length;
			vertexData_[start + 4].normal.x = vertexData_[start + 4].position.x;
			vertexData_[start + 4].normal.y = vertexData_[start + 4].position.y;
			vertexData_[start + 4].normal.z = vertexData_[start + 4].position.z;



			//点b(左上)
			vertexData_[start + 5].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon)) ;
			vertexData_[start + 5].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexData_[start + 5].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon));
			vertexData_[start + 5].position.w = 1.0f;
			vertexData_[start + 5].texCoord.x = u;
			vertexData_[start + 5].texCoord.y = v;
			vertexData_[start + 5].normal.x = vertexData_[start + 5].position.x;
			vertexData_[start + 5].normal.y = vertexData_[start + 5].position.y;
			vertexData_[start + 5].normal.z = vertexData_[start + 5].position.z;

			
			

#pragma endregion
			

		}
	
	
	
	}

	

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color;
	materialData_->lightingKinds= LightingType::NoneLighting;

	materialData_->uvTransform = Matrix4x4	::MakeIdentity4x4();

	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = Matrix4x4::MakeAffineMatrix(transform.scale,transform.rotate, transform.translate+sphereCondtion.center);
	//遠視投影行列
	Matrix4x4 viewMatrixSphere = Matrix4x4::MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSphere = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, static_cast<float>(windowsSetup_->GetClientWidth()), static_cast<float>(windowsSetup_->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSphere = Matrix4x4::Multiply(worldMatrixSphere, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData_->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixData_->World =Matrix4x4::MakeIdentity4x4();


	//コマンドを積む
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, materialResource_->GetGPUVirtualAddress());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);

	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, transformationMatrixResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	//trueだったらtextureSrvHandleGPU2_
	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2,useMonsterBall_?textureSrvHandleGPU2_:textureSrvHandleGPU_);
	//Light
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, directionalLightResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(SUBDIVISION_*SUBDIVISION_*6, 1u, 0u, 0u);

	


}

