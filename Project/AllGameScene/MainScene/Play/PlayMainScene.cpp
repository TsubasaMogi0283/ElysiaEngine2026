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
	audio_->Play(musicScoreData_.handle, false);
	musicLength_= audio_->GetAudioLength(musicScoreData_.handle);
}

void PlayMainScene::Update(){
	//再生時間を取得
	musicTime_ = audio_->GetPlayCurrentTime(musicScoreData_.handle);

	//プレイ中
	if (isPlay_) {
		//ポーズ
		PauseProcess();

		//楽曲を再生し終わったとき
		if (musicLength_ > musicTime_) {
			isPlay_ = false;
		}
	}
	else {
		//楽曲停止
		audio_->Stop(musicScoreData_.handle);
		//終了シーンへ
		mainScene_->ChangeMainScene(std::make_unique<EndMainScene>());
		return;
	}
	

#ifdef _DEBUG
	ImGui::Begin("PlayScene");
	ImGui::InputFloat("楽曲再生時間", &musicTime_);
	ImGui::InputFloat("楽曲の長さ", &musicLength_);
	ImGui::End();

	//デバッグ用でNを押したらプレイシーンへ
	if (input_->IsTriggerKey(DIK_M)) {
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

void PlayMainScene::PauseProcess(){
	if (isPause_) {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//解除
			audio_->Stop(musicScoreData_.handle);
			isPause_ = false;
		}
	} else {
		if (input_->IsTriggerKey(DIK_ESCAPE)) {
			//ESCAPEでポーズ
			audio_->Stop(musicScoreData_.handle);
			isPause_ = true;
		}
	}
}

