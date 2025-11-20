#pragma once
#include "Enemy.h"
#include "Arrow.h"
#include <vector>


class Camera;

//EnemyStateはEnemy.hで定義済み

class EnemyWizard : public Enemy
{
public:
	EnemyWizard();
	virtual ~EnemyWizard();

	void Init() override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);


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


	float attackTime = 90.0f;  //攻撃の時間
	float attackTimer = 0.0f;//攻撃を計るタイマー

	float coolTimer = 0.0f;//クールダウンを図るタイマー
	float coolTime = 180.0f;//クールタイム

	float catchDistance = 250.0f;//プレイヤーを見つける距離

};

