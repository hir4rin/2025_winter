#pragma once
#include "Scene.h"
class GameoverScene : public Scene
{
public:
	GameoverScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;
};

