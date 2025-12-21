#pragma once
#include "Vec2.h"
class TitleStar
{
public:
	/// <summary>
	/// 座標と半径
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="r"></param>
	TitleStar(Vec2 pos,float r);
	virtual ~TitleStar();

	void Update();
	void Draw();

private:
	int m_frame;
	Vec2 m_posA;
	Vec2 m_posB;
	Vec2 m_center;
	int m_handle = -1;

	float targetRadius;

	float m_radius;
	float m_speed;
	float  m_angle;
	
};

