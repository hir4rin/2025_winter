#pragma once
#include "Enemy.h"

class Camera;

enum class FishState
{
	First,//最初に現れるとき
	Walk,//歩いているとき
	Dead,//死んだとき
	Defeat,//倒されたとき
	Release,//解放されたとき

};

class Fish :public Enemy
{
public:
	Fish(Vec2 pos,int num);
	Fish(Vec2 pos,int num,int hp,FishState state);
	virtual ~Fish();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	
	

	int GetHp() { return m_hp; }

	void HitFishDamage(int damage);
	

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
	int m_angle;

};

