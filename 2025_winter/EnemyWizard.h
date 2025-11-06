#pragma once
#include "Enemy.h"




class EnemyWizard : public Enemy
{
public:
	EnemyWizard();
	virtual ~EnemyWizard();

	void Init() override;
	void Update()override;
	void Draw()override;
private:
	int charaIdx;
	int charaIdy;
	int m_animframe;
	EnemyState _state;
	void CheckHit();
};

