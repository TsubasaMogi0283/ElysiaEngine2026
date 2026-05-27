#include "Framework.h"

//リリースチェッカー
static D3DResourceLeakChecker checker;

//main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//フレームワークの生成
	std::unique_ptr<Kamaboko::Framework> kamaboko = std::make_unique<Kamaboko::Framework>();

	//実行
	kamaboko->Run();

	//終了
	return 0;
}
