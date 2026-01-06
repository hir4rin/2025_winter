#include "GameoverScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "TitleScene.h"
#include "SceneController.h"

GameoverScene::GameoverScene(SceneController& controller) : Scene(controller)
{
	update_ = &GameoverScene::NormalUpdate;
	draw_ = &GameoverScene::NormalDraw;
}

void GameoverScene::FadeInUpdate(Input&)
{
}
void GameoverScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}
void GameoverScene::FadeOutUpdate(Input&)
{
}
void GameoverScene::NormalDraw()
{
	DrawString(320, 240, "Game Over Scene", 0xffffff);
}
void GameoverScene::FadeInDraw()
{
}
void GameoverScene::FadeOutDraw()
{
}


void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);

}

void GameoverScene::Draw()
{
	
	(this->*draw_)();

}