#include "EndTitleScene.h"

#include <TitleScene/TitleScene.h>
#include <GameSceneManager.h>

void EndTitleScene::Initialize(){

}

void EndTitleScene::Update(){

	//閉じたらセレクトへ
	if (titleScene_->GetGameSceneManager()->GetTransition()->SetCloseTransition()) {
		titleScene_->SetIsProcessEnd(true);
	}
}
