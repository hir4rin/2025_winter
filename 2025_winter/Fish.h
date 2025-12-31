#pragma once
#include "Enemy.h"

class Camera;

enum class FishState
{
	First,
	Walk,


};

class Fish :public Enemy
{
public:
	Fish(Vec2 pos,int num);
	virtual ~Fish();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	
	

	int GetHp() { return m_hp; }

private:
	//赤のwalk
	void Walk1();
	//緑のwalk
	void Walk2();
	//青のwalk
	void Walk3();

private:
	//本人情報
	FishState m_state;
	int m_num;
	int charaIdx;
	int charaIdy;
	//時間系
	int m_firstFrame;
	int m_animFrame;
	int backFrame;
	bool m_isBack;
	int m_backCoolTime;

	//
	int m_hp;

	bool isDead = false;

};

