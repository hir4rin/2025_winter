#pragma once
#include "Character.h"

enum class EnemyState
{
	Normal,
	Attack,
	Damage
};
class Enemy :public Character
{
public :
	Enemy();
	virtual ~Enemy();

	void Init() override;
	void Update()override;
	void Draw()override;

};

