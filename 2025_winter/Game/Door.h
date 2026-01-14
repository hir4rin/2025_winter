#pragma once
#include "Vec2.h"
#include "Rect.h"


class Camera;

enum class StageID
{
	Fstage,
	Sstage,
	Tstage,
	Forthstage
};

class Door
{
public:
	Door(Vec2 pos);
	Door(Vec2 pos,StageID id);
	Door(Vec2 pos,StageID id,int num);
	virtual ~Door();
	void Init();
	void Update();
	void Draw(Camera& camera);

	const Rect& GetColRect() { return m_colRect; }

	void OutUpdate();
	StageID GetDoorID() { return m_id; }

	void StarDraw(Camera& camera);//星の数の描画

private:
	Vec2 m_pos;
	int m_handle;
	int m_starHandle=-1;
	Rect m_colRect;
	int targetStage;//どのステージへ行くか
	Vec2 spawnPos;//移動先での出現位置

	int charaIdx;
	int charaIdy;

	int starIdx;

	int m_frame;
	int m_bossFrame;

	StageID m_id;
	bool m_isStageSelect = false;
	int m_stage = -1;//ステージセレクトのドアの番号

};

