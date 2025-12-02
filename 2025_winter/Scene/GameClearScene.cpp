#include "GameClearScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "TitleScene.h"
#include "SceneController.h"




GameClearScene::GameClearScene(SceneController& controller) : Scene(controller)
{

}

void GameClearScene::Update(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}

void GameClearScene::Draw()
{
	DrawString(320, 240, "Game Clear Scene", 0xffffff);
}