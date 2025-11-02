#pragma once

class Player;

class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
private:
	Player* m_pPlayer;


};

