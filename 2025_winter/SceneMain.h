#pragma once
#include "Input.h"
#include "Camera.h"
#include <memory>


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

	void CheckItemWizard();
public:
	//-------------------------------------------------敵の数だけ引数が違うコンストラクタをつくる
	/// <summary>
	/// 通常状態の当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitNormal(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards);
	/// <summary>
	/// バーニングでの当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitBurning(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards);
	/// <summary>
	/// フローズンでの当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitFrozen(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards);

private:
	std::shared_ptr<Player> m_pPlayer;
	std::vector<std::shared_ptr<EnemyWizard>> m_pEnemyWizards;
	std::shared_ptr<Item> m_pItems;
	Item* m_pItem;
	std::vector<Arrow*> m_arrows;

	Bg* m_pBg;
	Character* m_pCharacter;


	Input input;
	Camera camera;//カメラのためのオブジェクト

};

