#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//画面サイズの変更
	SetGraphMode(1920, 1080, 32);

	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();
		//画面のクリア
		ClearDrawScreen();

		//裏画面と表画面を入れ替える
		ScreenFlip();

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}




	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}