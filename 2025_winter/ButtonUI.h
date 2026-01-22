#pragma once
#include "Vec2.h"
#include <string>

class Camera;


class ButtonUI
{
public:
	ButtonUI(std::string name,Vec2 pos);
	virtual ~ButtonUI();
	void Update();
	void Draw(Camera& camera);

private:
	Vec2 m_pos;
	int m_handle = -1;
	int m_handle2 = -1;
	int m_frame = 0;
	float drawX = 0;//文字の幅調整用
	float drawY = 0;//文字の幅調整用
};

