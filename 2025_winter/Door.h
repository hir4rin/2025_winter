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

	const Rect& GetColRect() { return m_colRect; }


private:
	Vec2 m_pos;
	int m_handle;
	Rect m_colRect;
	int targetStage;//どのステージへ行くか
	Vec2 spawnPos;//移動先での出現位置

	int charaIdx;
	int charaIdy;




};

