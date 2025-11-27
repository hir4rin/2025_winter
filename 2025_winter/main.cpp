#include "DxLib.h"
#include "Application.h"
#include "Input.h"
#include "Pad.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	auto& app = Application::GetInstance();
	if (!app.Init())
	{
		return -1;
	}
	app.Run();
	app.Terminate();
	return 0;

	////画面サイズの変更
	//SetGraphMode(1920, 1080, 32);
	////SetWindowSize(640, 480);

	//ChangeWindowMode(true);

	//if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	//{
	//	return -1;			// エラーが起きたら直ちに終了
	//}
	////ダブルバッファモード
	//SetDrawScreen(DX_SCREEN_BACK);

	//Input input;//入力のためのオブジェクト
	//
	//SceneMain* pSceneMain = new SceneMain;
	//pSceneMain->Init();
	//
	//
	//while (ProcessMessage() == 0)
	//{
	//	LONGLONG time = GetNowHiPerformanceCount();
	//	//画面のクリア
	//	ClearDrawScreen();
	//	Pad::Update();
	//	input.Update();
	//	
	//	if (CheckHitKey(KEY_INPUT_R))
	//	{
	//		if (pSceneMain != nullptr)
	//		{
	//			delete pSceneMain;
	//			pSceneMain = nullptr;
	//			pSceneMain = new SceneMain;
	//			pSceneMain->Init();
	//		}
	//	}

	//	pSceneMain->Update();
	//	pSceneMain->Draw();


	//	//裏画面と表画面を入れ替える
	//	ScreenFlip();
	//	if (CheckHitKey(KEY_INPUT_ESCAPE))//escapeで抜け出す
	//	{
	//		break;
	//	}

	//	//fpsを60に固定
	//	while (GetNowHiPerformanceCount() - time < 16667)
	//	{

	//	}

	//	if (CheckHitKey(KEY_INPUT_ESCAPE))
	//	{
	//		break;
	//	}
	//}


	//delete pSceneMain;

	//DxLib_End();				// ＤＸライブラリ使用の終了処理

	//return 0;				// ソフトの終了 
}