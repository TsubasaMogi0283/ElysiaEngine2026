#include "GameSceneManager.h"

#include <cassert>
#include <imgui.h>
#include <vector>

#include "Factory/GameSceneFactory/GameSceneFactory.h"


void Elysia::GameSceneManager::Initialize() {

	//シーンファクトリーの生成
	abstractSceneFactory_ = std::make_unique<GameSceneFactory>();
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Main");

#ifdef _DEBUG
	//デバッグ時はこっちに入れてね
	currentGamaScene_->SetGameManager(this);
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Main");
#endif // _DEBUG
	//譜面データ生成と読み込み
	scoreDataManager_ = std::make_unique<ScoreDataManager>();
	scoreDataManager_->Initialize();

	//各シーン
	currentGamaScene_->SetGameManager(this);
	currentGamaScene_->Initialize();

	//トランジション
	transition_ = std::make_unique<Transition>();
	transition_->Initialize();

}

void Elysia::GameSceneManager::ChangeScene(const std::string& sceneName) {

	//強制解放
	if (currentGamaScene_) {
		currentGamaScene_.reset();
	}

	//新しいシーンに遷移するためにPreの所に入っていたものを入れる
	preSceneName_ = currentSceneName_;
	//現在入っているシーン名を更新
	currentSceneName_ = sceneName;

	//シーンの値を取ってくる
	currentGamaScene_ = abstractSceneFactory_->CreateScene(currentSceneName_);
	//空ではない時初期化処理に入る
	assert(currentGamaScene_);
	//初期化
	currentGamaScene_->SetGameManager(this);
	currentGamaScene_->Initialize();

}

void Elysia::GameSceneManager::Update() {
	//更新
	currentGamaScene_->Update();
	transition_->Update();

#ifdef _DEBUG
	ImGui::Begin("ゲームシーンの管理");
	const char* SCENE_NAME[] = { "Title","Main","Win","Lose" };
	if (ImGui::BeginCombo("シーン", SCENE_NAME[currentSceneNumber_])) {
		for (uint32_t i = 0u; i < IM_ARRAYSIZE(SCENE_NAME); i++) {
			bool isSelected = false;
			if (currentSceneNumber_ == i) {
				isSelected = true;
			}

			if (ImGui::Selectable(SCENE_NAME[i], isSelected)) {
				// 選択されたアイテムのインデックスを更新する
				currentSceneNumber_ = i;
				ChangeScene(SCENE_NAME[i]);
			}

			// 現在選択されているアイテムにフォーカスを設定する
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}

		}

		ImGui::EndCombo();
	}

	ImGui::End();
#endif // _DEBUG
}

void Elysia::GameSceneManager::DrawObject3D() {
	//3Dオブジェクトの描画
	currentGamaScene_->DrawObject3D();
}

void Elysia::GameSceneManager::DrawSprite() {
	//スプライトの描画
	currentGamaScene_->DrawSprite();
	//トランジションの描画
	transition_->DrawSprite();
}

void Elysia::GameSceneManager::PreDrawPostEffect() {
	//ポストエフェクト描画処理前
	currentGamaScene_->PreDrawPostEffect();
}


void Elysia::GameSceneManager::DrawPostEffect() {
	//ポストエフェクト描画前
	currentGamaScene_->DrawPostEffect();
}



