#include "GameScene0.h"
#include "DxLib.h"
#include "../input.h"
#include "GameoverScene.h"
#include "SceneController.h"
#include "../Application.h"

constexpr int fade_interval = 60;




void GameScene0::FadeInUpdate(Input&)
{
	if (frame_-- <= 0)
	{
		update_ = &GameScene0::NormalUpdate;
		draw_ = &GameScene0::NormalDraw;
		return;
	}
}

void GameScene0::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		update_ = &GameScene0::FadeOutUpdate;
		draw_ = &GameScene0::FadeDraw;
		frame_ = 0;	// フェードアウトの最初
		return;
	}
}

void GameScene0::FadeOutUpdate(Input&)
{
	if (frame_++ >= fade_interval)
	{
		//controller_.ChangeScene(std::make_shared<GameoverScene>(controller_,));
		return;
	}
}

void GameScene0::NormalDraw()
{

	DrawFormatString(1920 / 2, 1080 / 2, 0xffffff, "Title Scene: Press 'OK' to Start");
	//const auto& wsize = Application::GetInstance().GetWindowSize();
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, titleH_, true);
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, titleLogoH_, true);
}

void GameScene0::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, titleH_, true);
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, titleLogoH_, true);
	//// 値の範囲をいったん0.0~1.0にしておくといろいろと扱いやすくなります
	auto rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate); // αブレンド
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);	// 画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドしない
}

GameScene0::GameScene0(SceneController& controller) : Scene(controller), frame_(0)
{
	/*titleH_ = LoadGraph(L"data/title.png");
	titleLogoH_ = LoadGraph(L"data/game_title.png");*/
	update_ = &GameScene0::FadeInUpdate;
	draw_ = &GameScene0::FadeDraw;
	frame_ = fade_interval;
}

void GameScene0::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene0::Draw()
{
	(this->*draw_)();
}