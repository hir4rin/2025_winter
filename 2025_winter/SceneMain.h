#pragma once
#include "Input.h"

class Player;
class EnemyWizard;
class Item;

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
	Item* m_pItem;



	Input input;

};

