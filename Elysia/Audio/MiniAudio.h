#pragma once

/**
 * @file MiniAudio.h
 * @brief miniaudioクラス
 * @author 茂木翼
 */

#include <cstdint>
#include <string>
#include <array>
#include <memory>
#include <miniaudio/miniaudio.h>

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// miniaudioクラス
	/// </summary>
	class MiniAudio final {
	private:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		MiniAudio() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~MiniAudio() = default;
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static MiniAudio* GetInstance();

		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="audio"></param>
		MiniAudio(const MiniAudio& audio) = delete;

		/// <summary>
		/// 代入演算子を無効にする
		/// </summary>
		/// <param name="audio"></param>
		/// <returns></returns>
		MiniAudio& operator=(const MiniAudio& audio) = delete;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();


	public:

		/// <summary>
		/// 読み込み
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <returns></returns>
		int32_t Load(const std::string& filePath);

		/// <summary>
		/// 再生
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <param name="isLoop">ループするかどうか</param>
		void Play(const int32_t audioHandle, const bool& isLoop);

		/// <summary>
		/// 一時停止
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		void Pause(const int32_t& audioHandle);

		/// <summary>
		/// 再開
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		void Resume(const int32_t& audioHandle);

		/// <summary>
		/// 停止(最初から)
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		void Stop(const int32_t& audioHandle);

		/// <summary>
		/// 再生時間の取得
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <returns>時間</returns>
		float_t GetPlayCurrentTime(const int32_t& audioHandle);

		/// <summary>
		/// 長さを取得
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <returns>時間の長さ</returns>
		float_t GetLength(const int32_t& audioHandle);

#pragma region ループ

	private:

		/// <summary>
		/// ループ設定
		/// </summary>
		/// <param name="sound">サウンドデータ</param>
		/// <param name="startTime">開始時間</param>
		/// <param name="endTime">終了時間</param>
		bool SetLoopRange(ma_sound* sound, float_t& startTime,const float_t& endTime);

	public:
		/// <summary>
		/// 前半ループ
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <param name="endTime">ループ終了時間</param>
		void BeforeLoop(const int32_t& audioHandle,const float_t& endTime);

		/// <summary>
		/// 後半ループ
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <param name="startTime">ループ開始時間</param>
		void AfterLoop(const int32_t& audioHandle,const float_t& startTime);

		/// <summary>
		/// 部分ループ
		/// </summary>
		/// <param name="audioHandle">ハンドル</param>
		/// <param name="startTime">開始時間</param>
		/// <param name="endTime">終了時間</param>
		void PartlyLoop(const int32_t& audioHandle, const float_t& startTime, const float_t& endTime);

		/// <summary>
		/// ループ解除
		/// </summary>
		/// <param name="audioHandle"></param>
		void ExitLoop(const int32_t& audioHandle);

#pragma endregion


	private:
		/// <summary>
		/// サウンドの取得
		/// </summary>
		/// <param name="handle">ハンドル</param>
		/// <returns>サウンド</returns>
		ma_sound* GetSound(const int32_t& handle) {
			//無い場合は空を返す
			if (!soundArray_[handle]) {
				return nullptr;
			}
			//見つかった時
			return soundArray_[handle]->sound.get();
		}

	public:
		/// <summary>
		/// 情報
		/// </summary>
		struct SoundData {
			//サウンド
			std::unique_ptr<ma_sound> sound = nullptr;
			//パス
			std::string path = "";
			//ハンドル
			int32_t handle = -1;

		};

	private:
		//サブミックスの数
		static const uint8_t SUBMIXVOICE_AMOUNT_ = 64u;
		//サウンドの数
		static const uint16_t SOUND_AMOUNT_ = 512u;

	private:
		//初期化済みかどうか
		bool isInitialized_ = false;

		//エンジン
		ma_engine engine_{};
		//サブミックス用
		std::array<ma_sound_group, SUBMIXVOICE_AMOUNT_> submixGroupArray_ = {};
		//サウンド
		std::array<std::unique_ptr<SoundData>, SOUND_AMOUNT_> soundArray_ = { };

	};
}
