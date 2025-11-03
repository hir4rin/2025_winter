#pragma once
#include "Vec2.h"
class Character
{
public:
	Character();
	virtual ~Character();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void Gravity();
protected:
	int m_handle;
	

	int m_x;
	int m_y;
	int m_jumpFrame;//ジャンプフレーム数

	bool m_isRight;//キャラクターが右を向いているかどうか
	bool m_isGround;//キャラクターが地面を向いているかどうか
	bool m_isJumpPreparing;//ジャンプ準備中か

	Vec2 m_pos;
	Vec2 m_vel;
	
};

