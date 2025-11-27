#pragma once
#include "Scene.h"
#include "Input.h"
#include "Camera.h"
#include "Enemy.h"
#include <memory>
#include <cassert>


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
class Door;

constexpr int cameraMargin = 100;
constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;


class GameScene : public Scene
{
public:
	GameScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;

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
	void CheckHitNormal();
	/// <summary>
	/// バーニングでの当たり判定チェック(遅いほうのバーニング)
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitBurning();
	/// <summary>
	///  バーニングでの当たり判定チェック(速いほうのバーニング)
	/// </summary>
	void CheckFastBurning();
	/// <summary>
	/// フローズンでの当たり判定チェック
	/// </summary>
	/// <param name="enemyWizards"></param>
	void CheckHitFrozen();

	/// <summary>
	/// プレイヤーが敵に当たっていないかどうか
	/// 当たったら敵も一緒に消える
	/// </summary>
	void CheckPlayer();

	


private:
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	using UpdateFunc_t = void(GameScene::*)(Input&);
	UpdateFunc_t update_;	// Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t draw_;	// Draw系を受け取るメンバ関数ポインタ


	//敵の位置情報のベクター(type,pos,spawned)
	std::vector<EnemySpawn> m_enemySpawns;
	//敵を生成するunique_ptrのベクター
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	//カメラの内側にいるかどうか
	bool IsInCamera(float x, float y)
	{
		float left = camera.pos.x - screenWidth/2 + 10;//???一度わからなくなった
		float right = camera.pos.x + screenWidth/2 - 10;//???一度わからなくなった
		float top = camera.pos.y - cameraMargin;
		float bottom = camera.pos.y + screenHeight + cameraMargin;


		return (x > left && x < right && y > top && y < bottom);
	}

	EnemySpawn& FindSpawnData(const Vec2& id)
	{
		for (auto& spawn : m_enemySpawns)
		{
			if (spawn.pos == id)return spawn;
		}
		assert(false && "posがちがうenemyが検出されました");
	}

	/// <summary>
	/// なんかよくわからんAIが考えた新しく矩形を作ってそれで判定
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="rect"></param>
	/// <returns></returns>
	bool CheckSweepHit(const Vec2& p0, const Vec2& p1, const Rect& rect);

private:
	int frame_ = 0;	// フェードインアウト用
	int gameFrame_ = 0;	// ゲーム中のフレームをカウント
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

	std::shared_ptr<Door> m_doors;


	
	Camera camera;//カメラのためのオブジェクト

};

