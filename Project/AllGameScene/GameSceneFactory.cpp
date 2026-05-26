#include "GameSceneFactory.h"

#include "TestScene/TestScene.h"

std::unique_ptr<Kamaboko::IGameScene> GameSceneFactory::CreateScene(const std::string& sceneName){

    //シーンを生成
    std::unique_ptr<Kamaboko::IGameScene> newScene = nullptr;

    //テスト用
    if (sceneName == "Test") {
        newScene = std::make_unique<TestScene>();
    }


    return newScene;
}
