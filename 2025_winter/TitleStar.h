#pragma once
#include "Vec2.h"
class TitleStar
{
public:
	TitleStar(Vec2 pos);
	virtual ~TitleStar();

	void Update();
	void Draw();

private:
	Vec2 m_pos;
	int m_handle = -1;;
	int m_angle;
};

