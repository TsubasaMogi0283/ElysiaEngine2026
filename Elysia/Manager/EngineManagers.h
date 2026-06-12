#pragma once

/**
 * @file EngineManagers.h
 * @brief エンジンの管理クラスの構造体。
 * シングルトンパターンを消したいため、構造体にまとめる。
 * @author 茂木翼
 */

#include <memory>
#include <WindowsSetup.h>
#include <DirectXSetup.h>
#include <SrvManager.h>
#include <RtvManager.h>
#include <ImGuiManager.h>
#include <PipelineManager.h>
#include <Input.h>
#include <Audio.h>
#include <GlobalVariables.h>
#include <LevelDataManager.h>

/// <summary>
/// エンジンの管理クラスをまとめた構造体
/// </summary>
struct EngineManagers {
	//ウィンドウクラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;
	//DirectXクラス
	Elysia::DirectXSetup* directXSetup_ = nullptr;
	//SRV管理クラス
	Elysia::SrvManager* srvManager_ = nullptr;
	//RTV管理クラス
	Elysia::RtvManager* rtvManager_ = nullptr;
	//ImGui管理クラス
	std::unique_ptr<Elysia::ImGuiManager> imGuiManager_ = nullptr;
	//パイプライン管理クラス
	Elysia::PipelineManager* pipelineManager_ = nullptr;
	//Inputクラス
	Elysia::Input* input_ = nullptr;
	//オーディオクラス
	Elysia::Audio* audio_ = nullptr;
	//JSON読み込み(グローバル変数)
	Elysia::GlobalVariables* globalVariables_ = nullptr;
	//レベルデータ管理クラス
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
};