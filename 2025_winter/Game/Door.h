#pragma once
#include "Vec2.h"
#include "Rect.h"


class Camera;

enum class StageID
{
	Fstage,
	Sstage,
	Tstage
	
};

class Door
{
public:
	Door(Vec2 pos);
	Door(Vec2 pos,StageID id);
	virtual ~Door();
	void Init();
	void Update();
	void Draw(Camera& camera);

	const Rect& GetColRect() { return m_colRect; }

	void OutUpdate();
	StageID GetDoorID() { return m_id; }

private:
	Vec2 m_pos;
	int m_handle;
	Rect m_colRect;
	int targetStage;//どのステージへ行くか
	Vec2 spawnPos;//移動先での出現位置

	int charaIdx;
	int charaIdy;

	int m_frame;

	StageID m_id;


};

