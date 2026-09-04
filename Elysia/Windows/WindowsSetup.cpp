#include "WindowsSetup.h"

#pragma comment(lib,"winmm.lib")

#include "Convert.h"


Elysia::WindowsSetup* Elysia::WindowsSetup::GetInstance(){
	static WindowsSetup instance;
	return &instance;
}

LRESULT Elysia::WindowsSetup::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	
	switch (msg) {
	case WM_ENTERSIZEMOVE:
		//ウィンドウが移動中
		Elysia::WindowsSetup::GetInstance()->isWindowMove_ = true;
		if (Elysia::WindowsSetup::GetInstance()->onEnterSizeMoveCallback_) {
			Elysia::WindowsSetup::GetInstance()->onEnterSizeMoveCallback_();
		}
		break;

	case WM_EXITSIZEMOVE:
		//ウィンドウが移動終了
		Elysia::WindowsSetup::GetInstance()->isWindowMove_ = false;
		if (Elysia::WindowsSetup::GetInstance()->onExitSizeMoveCallback_) {
			Elysia::WindowsSetup::GetInstance()->onExitSizeMoveCallback_();
		}
		break;

	case WM_DESTROY:
		//ウィンドウが破棄された
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Elysia::WindowsSetup::OutputStringW(std::string& stringText){
	//stringからwstringに変換
	std::wstring newString = Convert::Text::ToWString(stringText);
	//出力
	OutputDebugStringW(newString.c_str());
}

void Elysia::WindowsSetup::OutPutStringA(const std::string& text){
	//出力
	OutputDebugStringA(text.c_str());
}



#pragma region Initializeに入れる関数

//ウィンドウに情報を入れる
void  Elysia::WindowsSetup::RegisterWindowsClass(const wchar_t* title) {
	
	//ウィンドウプロシャージャ
	windowClass_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	windowClass_.lpszClassName = L"%s", title;
	//インスタンドハンドル
	windowClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラス登録
	RegisterClass(&windowClass_);

	//フルサイズにしたい場合のサイズを取得
	fullSizeX_ = GetSystemMetrics(SM_CXSCREEN);
	fullSizeY_ = GetSystemMetrics(SM_CYSCREEN);

	//ウィンドウのサイズを設定(左、上、横幅、立幅)
	RECT wrc = { 0,0,static_cast<LONG>(clientSize_.x), static_cast<LONG>(clientSize_.y) };
	// クライアント領域を元に実際のサイズにwrcを変更
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ生成
	hwnd_ = CreateWindow(
		//クラス名
		windowClass_.lpszClassName,
		//タイトルバーの文字
		title,
		//標準的なウィンドウスタイル
		WS_OVERLAPPEDWINDOW,
		//標準X座標
		CW_USEDEFAULT,
		//標準Y座標
		CW_USEDEFAULT,
		//横幅
		wrc.right - wrc.left,
		//縦幅
		wrc.bottom - wrc.top,
		//親ハンドル
		nullptr,
		//メニューハンドル
		nullptr,
		//インスタンスハンドル
		windowClass_.hInstance,
		//オプション
		nullptr					
	);
}


#pragma endregion

void Elysia::WindowsSetup::Initialize(const wchar_t* title, const Vector2<uint16_t>& clientSize) {
	//値を入れる
	clientSize_ = clientSize;

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	//ウィンドウクラスを登録
	RegisterWindowsClass(title);

	//ウィンドウを表示
	DisplayWindow();
}

void Elysia::WindowsSetup::WindowsMSG(MSG& msg) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}


