#pragma once
#include "Character.h"

class EnemyWizard;
class EnemyRider;
class Player;

enum class EnemyState
{
	Normal,
	Walk,
	Attack,
	Damage
};

enum class EnemyType
{
	Wizard,
	Rider,
	Archer
};

struct EnemySpawn
{
	EnemyType type;
	EnemyState state;
	Vec2 pos;
	bool spawned = false;
	bool wasKilled = false;
};;


class Enemy :public Character
{
public :
	Enemy();
	virtual ~Enemy();

	void Init() override;
	void Update()override;
	void Draw()override;


	//stateをDamageに変える
	void HitDamaged() { _state = EnemyState::Damage; }
	void DamageUpdate();
	bool GetisDead() { return m_isDead; }


	//ここは引き数を変えるだけにして、簡単にする
	/// <summary>
	/// ドロップアイテムを渡すよう
	/// </summary>
	/// <param name="enemy"></param>
	void DropItem(std::shared_ptr<EnemyWizard> enemy);
	void DropItem(std::shared_ptr<EnemyRider> enemy);

	void SetPlayer(std::shared_ptr<Player> player){m_pPlayer = player;}	
	void SetState(EnemyState state) { _state = state; }
	void  SetInitialID(Vec2 id) { m_initialID = id; }
	Vec2 GetInitialID() { return m_initialID; }

	std::shared_ptr<EnemyWizard> HitWizard = nullptr;//SceneMainに渡してアイテムを出させる
	std::shared_ptr<EnemyRider> HitRider = nullptr;//SceneMainに渡してアイテムを出させる
	std::shared_ptr<Player> m_pPlayer;

protected:
	Vec2 m_initialID;
	EnemyState _state;
	bool m_isDead;
protected:
	int m_damageFrame;

};

