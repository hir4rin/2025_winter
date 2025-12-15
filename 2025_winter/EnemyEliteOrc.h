#pragma once
#include "Enemy.h"

class Camera;


enum class AttackPattern
{
	
	Attack1,
	Attack2,
	Attack3
};

class EnemyEliteOrc : public Enemy
{
public:
	EnemyEliteOrc();
	~EnemyEliteOrc();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	void HitBossDamage(int damage);
	bool GetIsDead() { return isDead; }

private:
	int charaIdx;
	int charaIdy;
	int m_animframe;

	void CheckHit();


	void AnimChange(const EnemyState state);

	void NormalUpdate();
	void WalkUpdate();
	void AttackUpdate();
	void DamageUpdate();

	void AttackAnimIdxy();

	void Attack1();//実際に攻撃をする処理

	void Attack2();//実際に攻撃をする処理

	void Attack3();//実際に攻撃をする処理

	bool isAttack;//攻撃しているかどうか


	float attackTime = 180.0f;  //攻撃の時間


	float attackTimer = 0.0f;//攻撃を計るタイマー

	float coolTimer = 0.0f;//クールダウンを図るタイマー
	float coolTime = 180.0f;//クールタイム

	float catchDistance = 250.0f;//プレイヤーを見つける距離


private:
	AttackPattern m_attackP;
	int m_hp;
	bool isDead = false;
};

