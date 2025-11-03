#pragma once
#include "Character.h"
class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
private:
	void Move();//動きの入力
	void Jump();//ジャンプ
private:
	int m_frame;
};

