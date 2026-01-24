#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <cassert>
#include "input.h"
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
	//名前の変更
	SetMainWindowText("☆ジョブチェンジャーナイト☆");

	ChangeWindowMode(true);
	//フォントの読み込みを行う
	AddFontResourceEx("data/Font/x10y12pxDonguriDuel.ttf", FR_PRIVATE, NULL);



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	//音の初期化かつBgmの再生
	m_soundManager.Init();
	m_soundManager.PlayBgm("bgm");
	//------------------------------//
	// エフェクトの初期化
	//------------------------------//
	{
		// DirectX9を使用するようにする。(DirectX11も可)
		// Effekseerを使用するには必ず設定する。
		SetUseDirect3DVersion(DX_DIRECT3D_11);

		// Effekseerを初期化する。
		// 引数には画面に表示する最大パーティクル数を設定する。
		if (Effkseer_Init(16000) == -1)
		{
			DxLib_End();
			return -1;
		}

		// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
		// Effekseerを使用する場合は必ず設定する。
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

		// DXライブラリのデバイスロストした時のコールバックを設定する。
		// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
		// ただし、DirectX11を使用する場合は実行する必要はない。
		Effekseer_SetGraphicsDeviceLostCallbackFunctions();

		// Effekseerに2D描画の設定をする。
		Effekseer_Set2DSetting(1920, 1080);
		Effekseer_InitDistortion();//歪み

		// Zバッファを有効にする。
		// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
		SetUseZBuffer3D(TRUE);

		// Zバッファへの書き込みを有効にする。
		// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
		SetWriteZBuffer3D(TRUE);
	}
	return true;
}

void Application::Run()
{


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
	Effkseer_End();				// Effekseerを終了する。
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

