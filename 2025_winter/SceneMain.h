#pragma once
#include "Input.h"


class Player;
class EnemyWizard;
class Item;
class Arrow;

class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
	void CheckHit();
	void CheckArrowHit();
private:
	Player* m_pPlayer;
	std::vector<EnemyWizard*> m_pEnemyWizard;
	Item* m_pItem;
	std::vector<Arrow*> _arrow;


	Input input;

};

