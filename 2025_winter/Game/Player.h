#pragma once
#include "Character.h"
#include <vector>
#include <functional>

class Input;
class Arrow;

class EffekseerResourceManager;
class Bg;
class Camera;
enum class Anim
{
	Idle,
	Walk,
	Jump,
	Attack,
	Copy,
	Damage,
	Dead,
	StandUp

};
enum class PlayerState
{
	Normal,
	Attack,
	Copy,
	Damage

};

enum class PlayerType
{
	Normal,
	Burning,
	Frozen,
	Archer
};

class Player : public Character
{
public:
	/// <summary>
	/// 1:type,2:hp
	/// </summary>
	/// <param name="type"></param>
	/// <param name="hp"></param>
	Player(PlayerType type,int hp,Vec2 pos,int Life, std::shared_ptr<EffekseerResourceManager> effRes);
	//ゲームオーバー時用
	Player(PlayerType type,Vec2 pos, std::shared_ptr<EffekseerResourceManager> effRes);
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
	 PlayerState GetState() { return m_state; }//Playerの状態を取得
	PlayerType GetType() { return m_type; }//Playerの変身タイプを取得
	int GetAnimIdx() { return charaIdx; }//アニメーションのcharaIdxを取得
	void ChangeNormal();//外部から変身の変更
	void ChangeBurning();//外部から変身の変更
	void ChangeFrozen();//外部から変身の変更
	void ChangeArcher();//外部から変身の変更
	//無敵中かどうか
	bool CheckStar() { return damageTimer > 0; }

	//関数
	std::shared_ptr<Arrow> ShotArrow();//矢を発射させる

	int arrowFrame;//矢が発射するまでの時間
	bool isArrowAttack;//矢を撃ったかどうか

	Vec2 GetPrevPos() { return BurningPrevPos; }
	Vec2 GetAfterPos() { return BurningAfterPos; }
	
	bool isBurningAttack;///バーニング攻撃を下かどうか
	void ChangeBurningAttack(bool ans) { isBurningAttack = ans; }

	/// <summary>
	/// プレイヤーがダメージを食らったときに呼ぶ関数
	/// </summary>
	void DamageHit(bool ans);
	/// <summary>
	/// プレイヤーが回復した時に呼ぶ関数
	/// </summary>
	void HealGet(int point);

	/// <summary>
	/// 残機が減ったとき
	/// </summary>
	void LifeDeg();
	bool CheckLife() { return m_life <= 0; }
	void DroppedOutHp0() { m_hp = 0; }//落下で体力を0にする

	int  GetHp() { return m_hp; }//ｈｐ
	int GetLife() { return m_life; }//残機
	//画像を取得
	int GetHandle() { return m_handle; }
	

	void Death();
	void DyingUpdate();
	void DyingDraw(Camera& camera);

	void RotateUpdate();
	void RotateFinishUpdate();

	//m_animFrameを外部から更新するため
	void AnimFrameUpdate() { m_animframe++; }
	//アニメーションのみを外部から更新するため
	void AnimOnlyUpdate();


	//クリアシーンで動かすよう
	void AutoMoveStart();
	//大砲で自動移動中の処理
	void AutoMove();

	/// <summary>
	/// アニメーションが変わるところ
	/// </summary>
	/// <param name="anim"></param>
	void AnimSelect(const Anim& anim);//アニメーション
	void SetCopyCool() { m_copyCoolTime = 20; }//変身のコピークールタイム
public:
	//着地イベント
	std::vector<std::function<void()>> onLandEvents;
	//リスナー追加用(使いやすくするため)
	void AddOnLandEvent(const std::function<void()>& eventFunc) {
		onLandEvents.push_back(eventFunc);
	}
	std::vector<std::function<void()>> onWalkEvents;
	void AddOnWalkEvent(const std::function<void()>& eventFunc) {
		onWalkEvents.push_back(eventFunc);
	}
	std::vector<std::function<void()>> onDashEvents;
	void AddOnDashEvent(const std::function<void()>& eventFunc) {
		onDashEvents.push_back(eventFunc);
	}
	std::vector<std::function<void()>> onAttackEvents;
	void AddOnAttackEvent(const std::function<void()>& eventFunc) {
		onAttackEvents.push_back(eventFunc);
	}
	

private:
	void InputUpdate(Input& input);//入力の更新
	void NormalUpdate(Input& input);//通常時の更新
	void JumpUpdate(Input& input);//ジャンプ時の更新
	void AttackUpdate();//攻撃時の更新
	void CopyUpdate();//コピー時の更新
	void DamageUpdate();//ダメージを食らったときの更新


	 
	void ClearAttackRect();

	void Move(Input& input);//動きの入力の処理内容
	void Jump(Input& input);//ジャンプの処理内容
	void Attack();//攻撃のとこの処理内容

	void Copy();//剣を掲げるモーション
	
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

	bool MoveWithCollisionX(float distance);

public:
	bool GetTutorialFinish() { return m_tutorialFinished; }
	bool GetTutorialItemDrop() { return m_tutorialDrop; }
	void FalseTutorialItemDrop() { m_tutorialDrop = false; }
	//チュートリアルのアニメーション用
	bool m_tutorialFinished = false;
	void TutorialAttackUpdate();
	void TutorialCopyUpdate();
	void TutorialCopyOutUpdate();
	bool m_tutorialDrop = false;
	
	
private:
	int m_frame;
	int attackCoolTimer;
	int m_rotateFrame;//回転させるときのフレーム
	int m_junpTimer = 0;//クリアシーンで使う用のタイマー
	int m_jumpTime = 20;

	int m_wasGround;//前のフレームで地面にいたかどうか

	int m_rotateNum = 0;//回転した回数

	Anim m_anim;


	PlayerState m_state;
	PlayerType m_type;
	int m_hp;//playerの体力
	int m_life;//playerの残機
	int m_animframe;
	int m_copyCoolTime;
	int m_angle;//回転角度
	float damageCount;//ダメージを食らったときの秒数
	float damageTimer;//被ダメ秒数
	int charaIdx;
	int charaIdy;
	bool isNomove;
	bool isJumping;
	bool isRotateOne;
	
	const int arrowTime = 30;//発射クールタイム

	float burningTime = 9.0f;  //バーニングの移動時間
	float burningTimer = 0.0f;

	float coolTimer = 0.0f;
	float coolTime = 15.0f;//今のところ効いてない

	//バーニングの移動の前後の座標
	Vec2 BurningPrevPos;
	Vec2 BurningAfterPos;
	
	HitDir m_hitDir;

	//ノーマル
	Rect m_attackRect;//攻撃判定
	Rect m_copyRect;//コピー判定
	//バーニング
	Rect m_burningRect;//攻撃判定
	//フローズン
	Rect m_frozenRect;//攻撃判定
	//アーチャー
	Rect m_archerRect;//攻撃判定

private:
	//前回方向キーが押された時刻
     float m_lastTapTime;
	//前押した方向
	int m_lastTapDir;
	//ダッシュ中かどうか
	bool m_isDash;
		//2回押しの猶予時間
		const float doubleTapThreshold = 20.0f;
public:
//ゲームおーばー用
	void GameOverUpdate();
	void GameOverStandUpUpdate(float baseY);
	void AnimChangeStandUp();
	//ジャンプ用
	bool m_isTriJump = false;
	int m_triJumpFrame = 0;

private:
		int m_NormalAttackHandle = -1;
private:
	int m_burningEfHandle = -1;
	int playingEffectHandle = -1;//再生中のエフェクトハンドル
private:
	std::shared_ptr<EffekseerResourceManager> m_effRes;
	float drawOffsetOfcameraX = 0;
	float drawOffsetOfcameraY = 0;

};

