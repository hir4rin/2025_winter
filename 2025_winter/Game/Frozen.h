#pragma once
#include "Character.h"
#include <vector>
#include <memory>

class Camera;
class EnemyWizard;
class EnemyRider;
class EnemyArcher;
class EnemyArrow;
class BossShot;
class Salmon;
class Frozen :public Character
{
public:
	Frozen(std::shared_ptr<EnemyWizard> _enemyWiz);
	Frozen(std::shared_ptr<EnemyRider> _enemyRiders);
	Frozen(std::shared_ptr<EnemyArcher> _enemyArchers);
	Frozen(std::shared_ptr<EnemyArrow> _enemyArrow);
	Frozen(std::shared_ptr<BossShot> _bossShot);
	Frozen(std::shared_ptr<Salmon> _salmon);
	virtual ~Frozen();
	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	bool isMove = false;
public:
	
private:
	std::vector<std::shared_ptr<EnemyWizard>> _enemyWizard;

	 float kScale = 2.0f;
	 //Y座標を少し上げる
	  float drawY = 1.5f / 3.0f;
};

