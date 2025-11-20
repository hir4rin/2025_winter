#pragma once
#include "Character.h"
#include "vector"

class Input;
class Arrow;

class Bg;
class Camera;
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

enum class PlayerType
{
	Normal,
	Burning,
	Frozen,
	Archer
};;

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Update(Input& input);//すべてのプレイヤーの動き
	virtual void Draw() override;//すべてのプレイヤーの描画
	virtual void Draw(Camera& camera) ;//すべてのプレイヤーの描画(カメラ動かし)

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	/// <summary>
	/// 攻撃時の当たり判定を取得(Normal)
	/// </summary>
	/// <returns></returns>
	Rect GetColAttackRect() { return m_attackRect; }//攻撃判定を取得
	/// <summary>
	/// 攻撃時の当たり判定を取得(Burning)
	/// </summary>
	/// <returns></returns>
	Rect GetColBurningRect() { return m_burningRect; }
	/// <summary>
	/// 攻撃時の当たり判定を取得(Frozen)
	/// </summary>
	/// <returns></returns>
	Rect GetColFrozenRect() { return m_frozenRect; }
	/// <summary>
	/// 攻撃時の当たり判定を取得(Archer)
	/// </summary>
	/// <returns></returns>
	Rect GetColArcherRect() { return m_archerRect; }

	Rect GetColCopyRect() { return m_copyRect; }//攻撃判定を取得
	PlayerState GetState() { return _state; }//Playerの状態を取得
	PlayerType GetType() { return _type; }//Playerの変身タイプを取得
	int GetAnimIdx() { return charaIdx; }//アニメーションのcharaIdxを取得
	void ChangeNormal();//外部から変身の変更
	void ChangeBurning();//外部から変身の変更
	void ChangeFrozen();//外部から変身の変更
	void ChangeArcher();//外部から変身の変更

	//関数
	std::shared_ptr<Arrow> ShotArrow();//矢を発射させる

	int arrowFrame;//矢が発射するまでの時間
	bool isArrowAttack;//矢を撃ったかどうか



private:
	void InputUpdate(Input& input);//入力の更新
	void NormalUpdate(Input& input);//通常時の更新
	void JumpUpdate(Input& input);//ジャンプ時の更新
	void AttackUpdate();//攻撃時の更新
	void CopyUpdate();//コピー時の更新
	 

	void Move(Input& input);//動きの入力の処理内容
	void Jump(Input& input);//ジャンプの処理内容
	void Attack();//攻撃のとこの処理内容

	void Copy();//剣を掲げるモーション
	/// <summary>
	/// アニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelect(const Anim& anim );//アニメーション
	/// <summary>
	/// Normalのアニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelectNormal(const Anim& anim);
	/// <summary>
	/// Burningのアニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelectBurning(const Anim& anim);
	/// <summary>
	/// Frozenのアニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelectFrozen(const Anim& anim);
	/// <summary>
	/// Archerのアニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelectArcher(const Anim& anim);

	/// <summary>
	/// Normalのアニメーションを表示するところ
	/// </summary>
	void NormalAnim();
	/// <summary>
	/// Burningのアニメーションを表示するところ
	/// </summary>
	void BurningAnim();
	/// <summary>
	/// Frozenのアニメーションを表示するところ
	/// </summary>
	void FrozenAnim();
	/// <summary>
	/// Archerのアニメーションを表示するところ
	/// </summary>
	void ArcherAnim();

	
	
private:
	int m_frame;
	Anim _anim;
	PlayerState _state;
	PlayerType _type;
	int m_animframe;
	int charaIdx;
	int charaIdy;
	bool isNomove;
	const int arrowtime = 30;//発射クールタイム

	
	

	//ノーマル
	Rect m_attackRect;//攻撃判定
	Rect m_copyRect;//コピー判定
	//バーニング
	Rect m_burningRect;//攻撃判定
	//フローズン
	Rect m_frozenRect;//攻撃判定
	//アーチャー
	Rect m_archerRect;//攻撃判定
};

