#pragma once
#include "Character.h"
#include <memory>
class Camera;
class EnemyWizard;
class EnemyRider;
class EnemyArcher;

enum class ItemState
{
	Burning,
	Frozen,
	Archer

};

class Item :public Character
{
public:
	Item(std::shared_ptr<EnemyWizard> _enemyWiz);
	Item(std::shared_ptr<EnemyRider> _enemyRiders);
	Item(std::shared_ptr<EnemyArcher> _enemyArchers);
	virtual ~Item();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Draw(Camera& camera);

	ItemState GetItemState() const { return m_state; }

private:
	ItemState m_state;
};

