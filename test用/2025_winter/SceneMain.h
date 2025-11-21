#pragma once
#include "Input.h"
#include "Camera.h"
#include <memory>


class Player;
class EnemyWizard;
class EnemyRider;
class EnemyArcher;
class Item;
class Arrow;
class EnemyArrow;
class Bg;
class Character;
class Frozen;
class BurningObject;

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

	/// <summary>
	/// 氷(物理)の当たり判定チェック
	/// </summary>
	void CheckFrozenHit();

	/// <summary>
	/// バーニング(能力)に当たった時の敵の演出
	/// </summary>
	void ReactionBurning();

	
public:
	//-------------------------------------------------敵の数だけ引数が違うコンストラクタをつくる
	/// <summary>
	/// 通常状態の当たり判定チェック,
	/// アイテムもここで落ちる
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
	std::vector<std::shared_ptr<EnemyRider>>  m_pEnemyRiders;
	std::vector < std::shared_ptr<EnemyArcher>> m_pEnemyArchers;

	//アイテムは今のところ一つだけ
	std::shared_ptr<Item> m_pItems;
	Item* m_pItem;
	std::vector<std::shared_ptr<Arrow>> m_arrows;
	std::vector<std::shared_ptr<EnemyArrow>> m_pEnemyArrows;

	std::vector<std::shared_ptr<Frozen>> m_pFrozens;
	std::vector<std::shared_ptr<BurningObject>> m_pBurningObjects;
	 

	Bg* m_pBg;
	Character* m_pCharacter;


	Input input;
	Camera camera;//カメラのためのオブジェクト

};

