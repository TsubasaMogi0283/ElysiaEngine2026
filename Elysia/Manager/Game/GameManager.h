#pragma once
/**
 * @file GameManager.h
 * @brief ゲーム管理クラス
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>

#include "IGameScene.h"
#include "IAbstractSceneFactory.h"
#include <ScoreData/Manager/ScoreDataManager.h>
#include <Record/RecordManager.h>
#include <Note/NoteJudgement.h>


/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager {
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameManager() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// シーン遷移
		/// </summary>
		/// <param name="sceneName"></param>
		void ChangeScene(const std::string& sceneName);

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画(3Dオブジェクト)
		/// </summary>
		void DrawObject3D();

		/// <summary>
		/// 描画(スプライト)
		/// </summary>
		void DrawSprite();

		/// <summary>
		/// ポストエフェクト描画前処理
		/// </summary>
		void PreDrawPostEffect();

		/// <summary>
		/// 描画(ポストエフェクト)
		/// </summary>
		void DrawPostEffect();

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameManager() = default;

	public:
		/// <summary>
		/// 譜面管理クラスを取得
		/// </summary>
		/// <returns></returns>
		ScoreDataManager* GetScoreDataManager()const {
			return scoreDataManager_.get();
		}

		/// <summary>
		/// 一時記録場所
		/// </summary>
		/// <param name="musicInformation"></param>
		void SetMusicInformation(const MusicInformation& musicInformation) {
			this->temporaryMusicInformation_ = musicInformation;
		}

		/// <summary>
		/// 一時保存の楽曲情報を取得
		/// </summary>
		/// <returns></returns>
		MusicInformation GetMusicInformation()const {
			return temporaryMusicInformation_;
		}

		/// <summary>
		/// 判定結果の設定
		/// </summary>
		/// <param name="m"></param>
		inline void SetRecord(const NoteJudgement::Record& noteJudgementResult) {
			this->temporaryRecord_ = noteJudgementResult;
		}

		/// <summary>
		/// 判定結果の取得
		/// </summary>
		/// <returns></returns>
		inline NoteJudgement::Record GetNoteJudgementResult()const {
			return temporaryRecord_;
		}

		

	private:
		//シーンファクトリー
		std::unique_ptr<IAbstractSceneFactory> abstractSceneFactory_ = nullptr;

		//現在入っているシーン
		std::unique_ptr<IGameScene> currentGamaScene_ = nullptr;
		std::string  preSceneName_ = "";
		std::string  currentSceneName_ = "";

		//現在のシーンの数
		uint32_t currentSceneNumber_ = 0;

		//ノーツ管理クラス
		std::unique_ptr<ScoreDataManager> scoreDataManager_ = nullptr;
		std::unique_ptr<RecordManager> recordManager_ = nullptr;

	private:
		//一時保管場所
		//楽曲情報
		MusicInformation temporaryMusicInformation_ = {};
		//結果
		NoteJudgement::Record temporaryRecord_ = {};
	};
}