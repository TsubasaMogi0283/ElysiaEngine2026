#include "GameSceneFactory.h"

#include "TestScene/TestScene.h"
#include "TitleScene/TitleScene.h"
#include "SelectScene/SelectScene.h"
#include "MainScene/MainScene.h"
#include "ResultScene/ResultScene.h"

std::unique_ptr<Elysia::IGameScene> GameSceneFactory::CreateScene(const std::string& sceneName){

    //シーンを生成
    std::unique_ptr<Elysia::IGameScene> newScene = nullptr;

    if (sceneName == "Test") {
        //テスト
        newScene = std::make_unique<TestScene>();
    }
    else if (sceneName == "Title") {
        //タイトルシーン
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "Select") {
        //選択
        newScene = std::make_unique<SelectScene>();
    }
    else if (sceneName == "Main") {
        //メイン
        newScene = std::make_unique<MainScene>();
    }
    else if (sceneName == "Result") {
        //結果
        newScene = std::make_unique<ResultScene>();
    } 
    else {
        //その他テストへ
        newScene = std::make_unique<TestScene>();
    }

    return newScene;
}
