#pragma once
#include "Input.h"


class Player;
class EnemyWizard;
class Item;
class Arrow;
class Bg;
class Character;

class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
	/// <summary>
	/// 攻撃判定のチェック
	/// </summary>
	void CheckHit();
	/// <summary>
	/// 矢の当たり判定チェック
	/// </summary>
	void CheckArrowHit();
public:
	//-------------------------------------------------敵の数だけ引数が違うコンストラクタをつくる
	/// <summary>
	/// 通常状態の当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitNormal(std::vector<EnemyWizard*>& enemyWizards);
	/// <summary>
	/// バーニングでの当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitBurning(std::vector<EnemyWizard*>& enemyWizards);
	/// <summary>
	/// フローズンでの当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitFrozen(std::vector<EnemyWizard*>& enemyWizards);

private:
	Player* m_pPlayer;
	std::vector<EnemyWizard*> m_pEnemyWizard;
	Item* m_pItem;
	std::vector<Arrow*> m_arrows;

	Bg* m_pBg;
	Character* m_pCharacter;


	Input input;

};

