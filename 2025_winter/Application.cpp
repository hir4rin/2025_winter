#include "Application.h"
#include "DxLib.h"
#include <cassert>
#include "input.h"
#include "Input.h"
#include "Pad.h"
#include "Scene/SceneController.h"
#include "Scene/TitleScene.h"

constexpr int default_window_width = 1920;	// デフォルトウィンドウ幅
constexpr int default_window_height = 1080;	// デフォルトウィンドウ高
constexpr int default_color_bit = 32;	// デフォルトカラービット

Application::Application() :
	windowSize_{ default_window_width,default_window_height }
{

}
Application::~Application()
{
}

Application& Application::GetInstance()
{
	// staticなのでメモリの場所が一つで確定→これ一つしか実態がない→シングルトンクラス
	static Application instance;
	return instance;	// インスタンスの参照を返す→Applicationクラスの場所を返す
	// staticで一つしか存在しないものの参照を返すので複数になることはない
}

bool Application::Init()
{
	//画面サイズの変更
	SetGraphMode(1920, 1080, 32);
	//SetWindowSize(640, 480);

	ChangeWindowMode(true);
	//フォントの読み込みを行う
	AddFontResourceEx("data/Font/x10y12pxDonguriDuel.ttf", FR_PRIVATE, NULL);



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	//音の初期化かつBgmの再生
	m_soundManager.Init();
	m_soundManager.PlayBgm("bgm");
	return true;
}

void Application::Run()
{
	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	Input input;//入力のためのオブジェクト

	/*SceneMain* pSceneMain = new SceneMain;
	pSceneMain->Init();*/

	SceneController controller;
	controller.ChangeScene(std::make_shared<TitleScene>(controller));
	while (ProcessMessage() == 0 && !requestedExit_)
	{
		LONGLONG time = GetNowHiPerformanceCount();
		//画面のクリア
		ClearDrawScreen();
		Pad::Update();
		input.Update();

		if (CheckHitKey(KEY_INPUT_R))
		{
			//Rキーでシーンリセット
			/*if (pSceneMain != nullptr)
			{
				delete pSceneMain;
				pSceneMain = nullptr;
				pSceneMain = new SceneMain;
				pSceneMain->Init();
			}*/
		}
		controller.Update(input);
		controller.Draw();

		/*pSceneMain->Update();
		pSceneMain->Draw();*/


		//裏画面と表画面を入れ替える
		ScreenFlip();

			if (CheckHitKey(KEY_INPUT_ESCAPE))//escapeで抜け出す
		{
			break;
		}

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}
}

void Application::Terminate()
{
	//フォントの解放を行う
	RemoveFontResourceEx("data/Font/x10y12pxDonguriDuel.ttf", FR_PRIVATE, NULL);

	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	// TODO: return ステートメントをここに挿入します
	return windowSize_;
}

void Application::RequestExit()
{
	requestedExit_ = true;
}

