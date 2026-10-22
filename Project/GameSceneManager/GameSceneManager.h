#pragma once
/**
 * @file GameSceneManager.h
 * @brief ゲームシーン管理クラス
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>

#include "IGameScene.h"
#include "IAbstractSceneFactory.h"
#include <ScoreData/Manager/ScoreDataManager.h>
#include <Record/RecordManager.h>
#include <Note/NoteJudgement.h>
#include <Transition/Transition.h>

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// ゲームシーン管理クラス
	/// </summary>
	class GameSceneManager {
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameSceneManager() = default;

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
		~GameSceneManager() = default;

	public:
		/// <summary>
		/// 譜面管理クラスを取得
		/// </summary>
		/// <returns>譜面管理クラス</returns>
		ScoreDataManager* GetScoreDataManager()const {
			return scoreDataManager_.get();
		}

		/// <summary>
		/// トランジションクラスを取得
		/// </summary>
		/// <returns>トランジションクラス</returns>
		Transition* GetTransition()const {
			return transition_.get();
		}

		/// <summary>
		/// 一時譜面場所
		/// </summary>
		/// <param name="musicScoreData"></param>
		void SetTemporaryMusicScoreData(const ExistMusicScoreData& musicScoreData) {
			this->temporarymusicScoreData_ = musicScoreData;
		}

		/// <summary>
		/// 一時保存の譜面情報を取得
		/// </summary>
		/// <returns></returns>
		ExistMusicScoreData GetTemporaryMusicScoreData()const {
			return temporarymusicScoreData_;
		}

		/// <summary>
		/// 一時保存の難易度を記録
		/// </summary>
		/// <param name="difficulty">難易度</param>
		inline void SetTemporaryDifficulty(const int8_t& difficulty) {
			this->temporaryDifficulty_ = difficulty;
		}

		/// <summary>
		/// 一時保存の難易度を取得
		/// </summary>
		/// <returns>難易度</returns>
		inline int8_t GetTemporaryDifficulty()const {
			return temporaryDifficulty_;
		}

		/// <summary>
		/// ハンドルの設定
		/// </summary>
		/// <param name="musicHandle">楽曲ハンドル</param>
		/// <param name="jacketTextureHandle">ジャケットハンドル</param>
		inline void SetTemporaryHandle(const uint32_t& musicHandle, const uint32_t& jacketTextureHandle) {
			this->temporaryMusicHandle_ = musicHandle;
			this->temporaryJacketTextureHandle_ = jacketTextureHandle;
		}

		/// <summary>
		/// 楽曲ハンドルを取得
		/// </summary>
		/// <returns>楽曲ハンドル</returns>
		inline uint32_t GetTemporaryMusicHandle()const {
			return temporaryMusicHandle_;
		}

		/// <summary>
		/// ジャケットのテクスチャハンドルを取得
		/// </summary>
		/// <returns>テクスチャハンドル</returns>
		inline uint32_t GetTemporaryJacketTextureHandle()const {
			return temporaryJacketTextureHandle_;
		}

		/// <summary>
		/// 判定結果の設定
		/// </summary>
		/// <param name="m"></param>
		inline void SetTemporaryRecord(const NoteJudgement::Record& noteJudgementResult) {
			this->temporaryRecord_ = noteJudgementResult;
		}

		/// <summary>
		/// 判定結果の取得
		/// </summary>
		/// <returns></returns>
		inline NoteJudgement::Record GetNoteJudgementResult()const {
			return temporaryRecord_;
		}

		/// <summary>
		/// 記録の初期化
		/// </summary>
		inline void ResetRecord() {
			this->temporaryRecord_ = {};
		}

		/// <summary>
		/// ゲーム終了
		/// </summary>
		inline void SetIsGameEnd() {
			this->isGameEnd_ = true;
		}

		/// <summary>
		/// ゲーム終了状態を取得
		/// </summary>
		/// <returns>状態</returns>
		inline bool GetIsGameEnd()const {
			return isGameEnd_;
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

		//スコアデータ管理クラス
		std::unique_ptr<ScoreDataManager> scoreDataManager_ = nullptr;
		//レコード管理クラス
		std::unique_ptr<RecordManager> recordManager_ = nullptr;
		//トランジションクラス
		std::unique_ptr<Transition>transition_ = nullptr;

		//ゲーム終了
		bool isGameEnd_ = false;

	private:
		//一時保管場所
		ExistMusicScoreData temporarymusicScoreData_ = {};
		int8_t temporaryDifficulty_ = 0;
		uint32_t temporaryMusicHandle_ = 0u;
		uint32_t temporaryJacketTextureHandle_ = 0u;

		//結果
		NoteJudgement::Record temporaryRecord_ = {};
	};
}