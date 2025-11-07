#pragma once
#include "Character.h"

class Input;
enum class Anim
{
	Idle,
	Walk,
	Jump,
	Attack,
	Copy,

};
enum class PlayerState
{
	Normal,
	Attack,
	Copy

};

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Update(Input& input);
	virtual void Draw() override;

	/// <summary>
	/// 攻撃時の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	Rect GetColAttackRect() { return m_attackRect; }//攻撃判定を取得
	PlayerState GetState() { return _state; }//Playerの状態を取得
	int GetAnimIdx() { return charaIdx; }//アニメーションのcharaIdxを取得
private:
	void InputUpdate(Input& input);//入力の更新
	void NormalUpdate(Input& input);//通常時の更新
	void JumpUpdate(Input& input);//ジャンプ時の更新
	void AttackUpdate();//攻撃時の更新
	void CopyUpdate();//コピー時の更新
	 

	void Move(Input& input);//動きの入力
	void Jump(Input& input);//ジャンプ
	void Attack();//攻撃

	void Copy();//剣を掲げるモーション
	void AnimSelect(const Anim& anim );//アニメーション
private:
	int m_frame;
	Anim _anim;
	PlayerState _state;
	int m_animframe;
	int charaIdx;
	int charaIdy;
	bool isNomove;

	Rect m_attackRect;//攻撃判定
	Rect m_copyRect;//コピー判定
	
};

