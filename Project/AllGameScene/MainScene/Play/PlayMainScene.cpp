#include "PlayMainScene.h"

#include <imgui.h>

#include <Input.h>
#include <Audio.h>
#include <MainScene/MainScene.h>
#include <GameManager.h>
#include <MainScene/End/EndMainScene.h>
#include <Note/NormalTap/NormalTapNote.h>
#include <Note/Long/LongNote.h>

PlayMainScene::PlayMainScene(){
	//入力
	input_ = Elysia::Input::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();

}

void PlayMainScene::Initialize(){
	//メインシーンの空チェック
	assert(mainScene_);

	//楽曲情報を取得
	musicInformation_ = mainScene_->GetGameManager()->GetMusicInformation();
	musicScoreData_ = mainScene_->GetGameManager()->GetScoreDataManager()->GetSampleMusicScoreData();


	


	//楽曲の再生
	audio_->Play(musicInformation_.musicHandle, false);
}

void PlayMainScene::Update(){
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicInformation_.musicHandle);

	//プレイ中
	if (isPlay_) {
		//ポーズ
		if (isPause_) {

		}
	}
	else {

	}

	

#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_N)) {
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
	}

#endif // _DEBUG
}

void PlayMainScene::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	camera;
	baseLight;
}

void PlayMainScene::PreDrawPostEffect(){

}

void PlayMainScene::DrawPostEffect(){

}

void PlayMainScene::DrawSprite(){

}

