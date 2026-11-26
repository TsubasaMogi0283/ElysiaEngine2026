#pragma once

/**
 * @file DifficultySelectScene.h
 * @brief 難易度選択シーンクラス
 * @author 茂木翼
 */


#include <SelectScene/BaseSelectScene.h>
#include <Hsv.h>
#include <ScoreData/Difficulty.h>

/// <summary>
/// 難易度選択シーン
/// </summary>
class DifficultySelectScene :public BaseSelectScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DifficultySelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:
	//難易度格納チェック
	std::array<bool, static_cast<uint8_t>(Difficulty::Size)> isDataExist_ = {};


	//現在設定されている難易度
	Difficulty currentDifficultyNumber_ = Difficulty::Normal;
	//難易度Specialの色
	Hsv specialHsv_ = {};


	//楽曲選択へ戻るか
	bool isReturn_ = false;


	//決定音
	int32_t decideSEHandle_ = -1;
	//選択音
	int32_t selectSEHandle_ = -1;


};

