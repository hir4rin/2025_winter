#include "GameoverScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "TitleScene.h"
#include "SceneController.h"

GameoverScene::GameoverScene(SceneController& controller) : Scene(controller)
{
}

void GameoverScene::Update(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}

void GameoverScene::Draw()
{
	DrawString(320, 240, "Game Over Scene", 0xffffff);
}