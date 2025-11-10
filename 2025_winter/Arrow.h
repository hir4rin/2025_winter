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
	constexpr static int Num = 10;
	Vec2& GetPosition() { return m_pos; }
public:
	bool isAlive;
	Vec2 m_dir;
private:
	const float m_speed = 10;
	float	AliveLimitX = 1920.0f;
	const int kCharaSize = 100;
	
	const float kScale = 3;
	
};

