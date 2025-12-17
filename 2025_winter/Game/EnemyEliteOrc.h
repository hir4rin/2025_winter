#pragma once
#include "Enemy.h"
#include <functional>

class Camera;


enum class AttackPattern
{
	
	Attack1,//ぐるぐる攻撃
	Attack2,//ジャンプ攻撃
	Attack3,//もの投げ攻撃
	Down//弱い攻撃後、ダウンする
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
	bool GetIsThrow() { return isThrow; }
	

	int GetHp() { return m_hp; }

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
	/// <summary>
	/// ぐるぐる攻撃
	/// </summary>
	void Attack1();//実際に攻撃をする処理
	/// <summary>
	/// ジャンプ攻撃
	/// </summary>
	void Attack2();//実際に攻撃をする処理
	/// <summary>
	/// もの投げ攻撃
	/// </summary>
	void Attack3();//実際に攻撃をする処理
	/// <summary>
	/// 弱い攻撃後、ダウン状態になる
	/// </summary>
	void AttackDown();

	bool isAttack;//攻撃しているかどうか
	bool isThrow;//投げるかどうか



	float attackTimer = 0.0f;//攻撃を計るタイマー

	float coolTimer = 0.0f;//クールダウンを図るタイマー
	float coolTime = 180.0f;//クールタイム

	float catchDistance = 1250.0f;//プレイヤーを見つける距離

public:
	std::vector<std::function<void()>> onAttackEndEvents;
	void AddOnAttackEndEvent(const std::function<void()>& eventFunc) {
		onAttackEndEvents.push_back(eventFunc);
	}
private:
	int m_coolDamageTimer;
	AttackPattern m_attackP;
	int m_hp;

	bool isDead = false;

	//アニメーション用
	int baseFrame = 0;
	int phaseProgress = 0;
	//攻撃２用
	float targetX = 0.0f;//目標までの距離
	float m_startX = 0.0f;//攻撃するときの最初のX座標
	float offset = 50.0f;//プレイヤーの後ろ50ピクセル//ゆとり

	//ダウン時の揺れ用
	Vec2 m_drawoffset = {0,0};

};

