#pragma once

/**
 * @file WindowsSetup.h
 * @brief Windowsクラス
 * @author 茂木翼
 */

#include <Windows.h>
#include <WinUser.h>
#include <cstdint>
#include <string>
#include <functional>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx12.h>
#include <ImGui/imgui_impl_win32.h>

//extern...グローバル変数を共有する

/// <summary>
///	ウィンドウプロシージャで使うもの
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// Windowsの設定
	/// </summary>
	class WindowsSetup final {
	private:
		//インスタンスを作れないようにする
		//コンストラクタをprivateに

		/// <summary>
		/// コンストラクタ
		/// </summary>
		WindowsSetup() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~WindowsSetup() = default;

	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static WindowsSetup* GetInstance();


		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="winApp"></param>
		WindowsSetup(const WindowsSetup& winApp) = delete;

		/// <summary>
		/// 代入演算子を無効にする
		/// </summary>
		/// <param name="winApp"></param>
		/// <returns></returns>
		WindowsSetup& operator=(const WindowsSetup& winApp) = delete;

	public:

		/// <summary>
		/// ウィンドウプロシージャ
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="msg"></param>
		/// <param name="wparam"></param>
		/// <param name="lparam"></param>
		/// <returns></returns>
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		/// <summary>
		/// ログ
		/// </summary>
		/// <param name="stringText"></param>
		static void OutputStringW(std::string& stringText);

		/// <summary>
		/// ログ
		/// </summary>
		/// <param name="text"></param>
		static void OutPutStringA(const std::string& text);

		/// <summary>
		/// ウィンドウが移動中かどうかを取得
		/// </summary>
		/// <returns>移動中かどうか</returns>
		inline bool GetIsWindowMove() const {
			return isWindowMove_;
		}

	private:

		/// <summary>
		/// ウィンドウクラスを登録
		/// </summary>
		/// <param name="title">タイトル名</param>
		void RegisterWindowsClass(const wchar_t* title);

		/// <summary>
		/// Windowを表示
		/// </summary>
		inline void DisplayWindow() const {
			//ウィンドウを表示
			ShowWindow(hwnd_, SW_SHOW);
		};


	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="title">タイトル名</param>
		/// <param name="clientWidth">横サイズ</param>
		/// <param name="clientHeight">縦サイズ</param>
		void Initialize(const wchar_t* title,const int32_t& clientWidth,const int32_t& clientHeight);

		/// <summary>
		/// メッセージ
		/// </summary>
		/// <param name="msg">メッセージ</param>
		void WindowsMSG(MSG& msg);

		/// <summary>
		/// 閉じる
		/// </summary>
		inline void Close() const {
			//ウィンドウを閉じる
			CloseWindow(hwnd_);
		};

	public:

		/// <summary>
		/// クライアントの横幅
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetClientWidth()const {
			return clientWidth_;
		}

		/// <summary>
		/// クライアントの縦幅
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetClientHeight() const{
			return clientHeight_;
		}

		/// <summary>
		/// Hwndの取得
		/// </summary>
		/// <returns></returns>
		inline HWND GetHwnd()const {
			return hwnd_;
		}

		/// <summary>
		/// hInstanceの取得
		/// </summary>
		/// <returns></returns>
		inline HINSTANCE GetHInstance() const {
			return windowClass_.hInstance;
		}

	public:
		//コールバック関数の設定
		void SetOnEnterSizeMoveCallback(std::function<void()> callback) { onEnterSizeMoveCallback_ = callback; }
		void SetOnExitSizeMoveCallback(std::function<void()> callback) { onExitSizeMoveCallback_ = callback; }

	private:
		//動く開始
		std::function<void()> onEnterSizeMoveCallback_ = nullptr;
		//動き終了
		std::function<void()> onExitSizeMoveCallback_ = nullptr;

	public:
		//クライアントのサイズ
		uint32_t clientWidth_ = 0u;;
		uint32_t clientHeight_ = 0u;


	private:
		//ウィンドウハンドル
		HWND hwnd_ = {};
		//ウィンドウクラス
		WNDCLASS windowClass_{};

		//ウィンドウが移動中かどうか
		bool isWindowMove_ = false;	

		//フルサイズ
		int32_t fullSizeX_ = 0;
		int32_t fullSizeY_ = 0;
	};
};
