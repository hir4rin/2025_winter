#pragma once
#include "Scene.h"
#include "Camera.h"

class Cannon;
class Bg;

class GameClearScene :public Scene
{
public:
	GameClearScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;


private:
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Cannon> m_pCannon;
	Camera camera;
	Bg* m_pBg;

	bool isCannon = false;
};

