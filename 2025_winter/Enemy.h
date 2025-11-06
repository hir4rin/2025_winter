#pragma once
#include "Character.h"
class Enemy :public Character
{
public :
	Enemy();
	virtual ~Enemy();

	void Init() override;
	void Update()override;
	void Draw()override;

};

