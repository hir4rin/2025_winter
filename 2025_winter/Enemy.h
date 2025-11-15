#pragma once
#include "Character.h"

class EnemyWizard;

enum class EnemyState
{
	Normal,
	Walk,
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
	void DropItem(std::shared_ptr<EnemyWizard> enemy);

	std::shared_ptr<EnemyWizard> HitWizard = nullptr;//SceneMainに渡してアイテムを出させる
};

