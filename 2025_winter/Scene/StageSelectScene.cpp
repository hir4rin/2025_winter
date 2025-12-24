#include "StageSelectScene.h"
#include "DxLib.h"
#include "Player.h"
#include "../input.h"
#include "GameScene.h"
#include "SceneController.h"
#include "../Application.h"




namespace
{
	constexpr int fade_interval = 60;
	constexpr int selectMax = 1;//最大-1

	constexpr int margin = 200;
}


void StageSelectScene::FadeInUpdate(Input&)
{
	if (frame_-- <= 0)
	{
		update_ = &StageSelectScene::NormalUpdate;
		draw_ = &StageSelectScene::NormalDraw;
		return;
	}
}

void StageSelectScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("up"))
	{
		//selectIndexを上げる
		selectIndex--;
		if (selectIndex < 0)
		{
			selectIndex = 0;
		}
	}
	if (input.IsTriggered("down"))
	{
		//selectIndexを下げる
		selectIndex++;
		if (selectIndex > selectMax)
		{
			selectIndex = selectMax;
		}
	}

	if (input.IsTriggered("Jump"))//決定
	{
		if (selectIndex+1 == 1)//ステージ1
		{
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, 1, PlayerType::Normal, 100,5));
			return;
		}
		if (selectIndex+1 == 2)//ステージ2
		{
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, 6, PlayerType::Normal, 100,5));
			return;
		}

	}
}

void StageSelectScene::FadeOutUpdate(Input&)
{
	
}

void StageSelectScene::NormalDraw()
{

	const auto& wsize = Application::GetInstance().GetWindowSize();

	DrawFormatString(wsize.w / 2 - margin, wsize.h/2 + (selectIndex) * margin, GetColor(255, 255, 255), "⇒");



	DrawFormatString(wsize.w / 2, wsize.h/2, GetColor(255, 255, 255), "stage_1");
	DrawFormatString(wsize.w / 2, wsize.h/2 + margin, GetColor(255, 255, 255), "stage_2");

	

	
}

void StageSelectScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	//// 値の範囲をいったん0.0~1.0にしておくといろいろと扱いやすくなります
	auto rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate); // αブレンド
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);	// 画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドしない
}

StageSelectScene::StageSelectScene(SceneController& controller,int Life) : Scene(controller)
{
	
	update_ = &StageSelectScene::NormalUpdate;
	draw_ = &StageSelectScene::NormalDraw;

	frame_ = fade_interval;

	


}

void StageSelectScene::Update(Input& input)
{
	(this->*update_)(input);
}

void StageSelectScene::Draw()
{
	(this->*draw_)();
}