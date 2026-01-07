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

	//アイテムが落ちたときの演出
	void DroppedUpdate();
	void DroppedDraw(Camera& camera);

	bool IsDead() { return m_aliveFrame <= 0; }
	bool IsInitDead() { return m_initAliveFrame <= 0; }
	

private:
	//アイテムの存在時間
	int m_initAliveFrame;
	ItemState m_state;
	//演出が生きてる時間
	int m_aliveFrame = 0;
	int m_angle = 0;
};

