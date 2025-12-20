#pragma once
#include "Vec2.h"
class TitleLightEffect
{

public:
	TitleLightEffect(Vec2 pos);
	virtual ~TitleLightEffect();

	void Update();
	void Draw();
	bool GetIsDead() { return m_isDead; }
private:
	int m_animFrame;
	int m_handle = -1;
	int m_handleDaen = -1;
	Vec2 m_pos;
	bool m_isDead;

	int charaIdx;
	int charaIdy;
};

