#pragma once
#include "Enemy.h"
#include <functional>
class Camera;


enum class BearAttackPattern
{
	Attack1,//連続ひっかき攻撃
	Attack2,//ジャンプ波動
	Attack3,//
	Down//
};

class EnemyBear :public Enemy
{
public:
	EnemyBear();
	virtual  ~EnemyBear();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

	void HitBossDamage(int damage);
	bool GetIsDead() { return isDead; }
	bool GetIsThrow() { return isThrow; }


	int GetHp() { return m_hp; }


	Rect& GetColAttack1Rect() { return m_attack1Rect; }

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
	/// 
	/// </summary>
	void Attack1();//実際に攻撃をする処理
	/// <summary>
	/// 
	/// </summary>
	void Attack2();//実際に攻撃をする処理
	/// <summary>
	/// 
	/// </summary>
	void Attack3();//実際に攻撃をする処理
	/// <summary>
	/// 弱い攻撃後、ダウン状態になる
	/// </summary>
	void AttackDown();

	BearAttackPattern SelectAttack();

	bool isAttack;//攻撃しているかどうか
	bool isThrow;//投げるかどうか



	float attackTimer = 0.0f;//攻撃を計るタイマー

	float coolTimer = 0.0f;//クールダウンを図るタイマー
	float coolTime = 180.0f;//クールタイム

	float catchDistance = 1250.0f;//プレイヤーを見つける距離

public:
	std::vector<std::function<void()>> onAttack1EndEvents;
	/// <summary>
	/// 攻撃1終わりのエフェクトなどを追加
	/// </summary>
	/// <param name="eventFunc"></param>
	void AddOnAttack1EndEvent(const std::function<void()>& eventFunc) {
		onAttack1EndEvents.push_back(eventFunc);
	}
	std::vector<std::function<void()>> onAttackEndEvents;
	/// <summary>
	/// 攻撃2終わりのエフェクトなどを追加
	/// </summary>
	/// <param name="eventFunc"></param>
	void AddOnAttackEndEvent(const std::function<void()>& eventFunc) {
		onAttackEndEvents.push_back(eventFunc);
	}
	std::vector<std::function<void()>> onAttack3EndEvents;
	/// <summary>
	/// 攻撃2終わりのエフェクトなどを追加
	/// </summary>
	/// <param name="eventFunc"></param>
	void AddOnAttack3EndEvent(const std::function<void()>& eventFunc) {
		onAttack3EndEvents.push_back(eventFunc);
	}

	std::vector<std::function<void()>> onDeathEvents;
	/// <summary>
	/// 死んだときのエフェクトなどを追加
	/// </summary>
	/// <param name="eventFunc"></param>
	void AddOnDeathEvent(const std::function<void()>& eventFunc) {
		onDeathEvents.push_back(eventFunc);
	}

private:
	int m_coolDamageTimer;
	BearAttackPattern m_attackP;
	int m_hp;

	bool isDead = false;

	//アニメーション用
	int baseFrame = 0;
	int phaseProgress = 0;

	//攻撃判定初期化用
	void ClearAttackRect();

	//攻撃1用
	Rect m_attack1Rect;

	//攻撃２用
	float targetX = 0.0f;//目標までの距離
	float m_startX = 0.0f;//攻撃するときの最初のX座標
	float offset = 50.0f;//プレイヤーの後ろ50ピクセル//ゆとり

	//ダウン時の揺れ用
	Vec2 m_drawoffset = { 0,0 };
};

