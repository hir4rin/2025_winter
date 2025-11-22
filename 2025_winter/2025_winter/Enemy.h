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
class Enemy :public Character
{
public :
	Enemy();
	virtual ~Enemy();

	void Init() override;
	void Update()override;
	void Draw()override;
	//ここは引き数を変えるだけにして、簡単にする
	/// <summary>
	/// ドロップアイテムを渡すよう
	/// </summary>
	/// <param name="enemy"></param>
	void DropItem(std::shared_ptr<EnemyWizard> enemy);
	void DropItem(std::shared_ptr<EnemyRider> enemy);

	void SetPlayer(std::shared_ptr<Player> player){m_pPlayer = player;}	

	std::shared_ptr<EnemyWizard> HitWizard = nullptr;//SceneMainに渡してアイテムを出させる
	std::shared_ptr<EnemyRider> HitRider = nullptr;//SceneMainに渡してアイテムを出させる
	std::shared_ptr<Player> m_pPlayer;

};

