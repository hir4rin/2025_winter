#pragma once
#include "Enemy.h"

class Camera;

class Fish :public Enemy
{
public:
	Fish(Vec2 pos,int num);
	virtual ~Fish();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	bool GetIsDead() { return isDead; }

	int GetHp() { return m_hp; }

private:
	int m_num;
	int charaIdx;
	int charaIdy;
	int m_animframe;
	int m_hp;

	bool isDead = false;

};

