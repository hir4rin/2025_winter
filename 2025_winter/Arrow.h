#pragma once
#include "Character.h"
#include <vector>

class EnemyWizard;

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
	
	void SetEnemyWizard(std::vector<EnemyWizard*>& wizards);//敵のポインタを渡す
	EnemyWizard* hitEnemy = nullptr;//当たって敵を一時保存
public:
	bool isAlive;
	Vec2 m_dir;
private:
	const float m_speed = 10;
	float	AliveLimitX = 1920.0f;
	const int kCharaSize = 100;
	
	const float kScale = 3;
	std::vector<EnemyWizard*> _enemyWizard;

};

