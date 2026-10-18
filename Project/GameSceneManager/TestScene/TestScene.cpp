#include "TestScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"

TestScene::TestScene(){
	//インスタンスの取得	
	//入力
	input_ = Elysia::Input::GetInstance();
}

void TestScene::Initialize(){
	

	//カメラ
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float>/6.0f;
	camera_.translate = { .x = 0.0f,.y = 21.0f,.z = -40.0f };
	//マテリアルの初期化
	playerMaterial_.Initialize();
	playerMaterial_.lightingKinds = LightingType::DirectionalLighting;
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	backTexture_->Initialize();
}

void TestScene::Update(){

	camera_.Update();
	directionalLight_.Update();
	playerMaterial_.Update();

#ifdef _DEBUG

	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG


}

void TestScene::DrawObject3D(){

}

void TestScene::PreDrawPostEffect(){
	//描画前処理
	backTexture_->PreDraw();
}

void TestScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void TestScene::DrawSprite(){

}


