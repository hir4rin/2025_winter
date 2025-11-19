#pragma once
#include "Enemy.h"
#include "Arrow.h"
#include <vector>

class Camera;

//EnemyStateはEnemy.hで定義済み

class EnemyArcher : public Enemy
{
public:
	EnemyArcher();
	virtual ~EnemyArcher();

	void Init() override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	//矢はプレイヤーと同じようにさせる
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

