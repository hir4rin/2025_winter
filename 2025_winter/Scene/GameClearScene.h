#pragma once
#include "Scene.h"

class GameClearScene :public Scene
{
public:
	GameClearScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;
};

