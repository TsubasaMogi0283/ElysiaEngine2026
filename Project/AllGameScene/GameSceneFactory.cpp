#include "GameSceneFactory.h"

#include "TestScene/TestScene.h"
#include "TitleScene/TitleScene.h"
#include "SelectScene/SelectScene.h"
#include "MainScene/MainScene.h"
#include "ResultScene/ResultScene.h"

std::unique_ptr<Elysia::IGameScene> GameSceneFactory::CreateScene(const std::string& sceneName){

    //シーンを生成
    std::unique_ptr<Elysia::IGameScene> newScene = nullptr;

    //テスト
    if (sceneName == "Test") {
        newScene = std::make_unique<TestScene>();
    }
	//タイトルシーン
    else if (sceneName == "Title") {
        newScene = std::make_unique<TitleScene>();
    }
    //選択
    else if (sceneName == "Select") {
        newScene = std::make_unique<SelectScene>();
    }
    //メイン
    else if (sceneName == "Main") {
        newScene = std::make_unique<MainScene>();
    }
    //結果
    else if (sceneName == "Result") {
        newScene = std::make_unique<ResultScene>();
    } 
    else {
        newScene = std::make_unique<TestScene>();
    }

    return newScene;
}
