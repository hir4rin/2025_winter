#pragma once
#include "Character.h"

class EnemyWizard;

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
	void DropItem(EnemyWizard* enemy);

	EnemyWizard* HitWizard = nullptr;//SceneMainに渡してアイテムを出させる
};

