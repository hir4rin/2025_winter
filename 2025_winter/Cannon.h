#pragma once
#include <Rect.h>


class Camera;

class Cannon
{
public:
	Cannon();
	virtual ~Cannon();
	void Init();
	void Update();
	void Draw(Camera& camera);
	void Fire();

	Rect& GetColRect() { return m_colRect; }

private:
	Rect m_colRect;
	int m_handle;
	int m_animFrame;
	int charaIdx = 0;
	int charaIdy = 0;

	Vec2 m_pos;


};

