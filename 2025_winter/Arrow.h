#pragma once
#include "Character.h"


class Arrow :public Character
{

public:
	Arrow();
	virtual ~Arrow();
	void Init()override;
	void Update()override;
	void Draw()override;
	const int Num = 5;
public:
	bool isAlive;
	Vec2 m_dir;
private:
	float m_speed;
	float	AliveLimitX = 1920.0f;
	const int kCharaSize = 100;
	
	const float kScale = 3;
	
};

