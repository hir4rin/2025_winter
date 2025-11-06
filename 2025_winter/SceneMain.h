#pragma once

class Player;
class EnemyWizard;

class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
	void CheckHit();
private:
	Player* m_pPlayer;
	EnemyWizard* m_pEnemyWizard;


};

