#pragma once
#include "Enemy.h"
#include "Arrow.h"
#include <vector>


class Bg;

//EnemyStateはEnemy.hで定義済み

class EnemyWizard : public Enemy
{
public:
	EnemyWizard();
	virtual ~EnemyWizard();

	void Init() override;
	void Update()override;
	void Draw()override;


private:
	int charaIdx;
	int charaIdy;
	int m_animframe;
	EnemyState _state;
	void CheckHit();

	void AnimChange(const EnemyState state);

	void NormalUpdate();
	void WalkUpdate();
	void AttackUpdate();

	void Attack();//実際に攻撃をする処理
	
	bool isAttack;//攻撃しているかどうか

};

