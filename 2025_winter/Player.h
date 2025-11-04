#pragma once
#include "Character.h"


enum class Anim
{
	Idle,
	Walk,
	Jump,
	Attack

};

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
	void AnimSelect(const Anim& anim );//アニメーション
private:
	int m_frame;
	Anim _anim;
};

