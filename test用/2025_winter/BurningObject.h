#pragma once
#include "Character.h"
#include <vector>
#include <memory>

class Camera;
class EnemyWizard;
class EnemyRider;
class EnemyArcher;
class BurningObject:public Character
{
public:
	BurningObject(std::shared_ptr<EnemyWizard> _enemyWiz);
	BurningObject(std::shared_ptr<EnemyRider> _enemyRiders);
	BurningObject(std::shared_ptr<EnemyArcher> _enemyArchers);
	virtual ~BurningObject();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	bool m_burningObjectDir;
private:
	std::vector<std::shared_ptr<EnemyWizard>> _enemyWizard;



};

