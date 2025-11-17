#pragma once
#include "Character.h"
#include <vector>
#include <memory>

class Camera;
class EnemyWizard;
class Frozen :public Character
{
public:
	Frozen(std::shared_ptr<EnemyWizard> _enemyWiz);
	virtual ~Frozen();
	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	bool isMove = false;
public:
	
private:
	std::vector<std::shared_ptr<EnemyWizard>> _enemyWizard;


};

