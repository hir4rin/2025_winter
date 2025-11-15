#pragma once
#include "Character.h"
#include <vector>
#include <memory>

class EnemyWizard;
class Player;
class Arrow :public Character
{

public:
	Arrow();
	virtual ~Arrow();
	void Init()override;
	void Update()override;
	void Draw()override;
	constexpr static int Num = 10;
	Vec2& GetPosition() { return m_pos; }
	
	void SetEnemyWizard(std::vector<std::shared_ptr<EnemyWizard>>& wizards);//敵のポインタを渡す
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer;}
	std::shared_ptr<EnemyWizard>  hitEnemy = nullptr;//当たって敵を一時保存
public:
	bool isAlive;
	Vec2 m_dir;
	bool m_playerdir;
private:
	const float m_speed = 10;
	float	AliveLimitX = 1920.0f;
	const int kCharaSize = 100;
	
	const float kScale = 3;
	std::vector<std::shared_ptr<EnemyWizard>> _enemyWizard;
	std::shared_ptr<Player> m_pPlayer;

};

