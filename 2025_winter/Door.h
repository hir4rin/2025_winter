#pragma once
#include "Vec2.h"
#include "Rect.h"


class Camera;
class Door
{
public:
	Door();
	virtual ~Door();
	void Init();
	void Update();
	void Draw(Camera& camera);


private:
	Vec2 m_pos;
	int m_handle;
	Rect m_colRect;




};

