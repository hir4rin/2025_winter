#pragma once
#include "Scene.h"
#include "Input.h"
#include "Camera.h"
#include "StageUI.h"
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



class GameScene1_3 :
    public Scene
{

public:
	GameScene1_3(SceneController& controller,PlayerType type,int hp);
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
	/// <summary>
	/// プレイヤーが落下した後の復帰処理
	/// </summary>
	bool CheckDropped();
	/// <summary>
	/// プレイヤーが死ぬ処理
	/// </summary>
	void DyingAct();
	/// <summary>
	/// カメラが揺れる(敵と当たったとき用)
	/// </summary>
	void OnShake();
private:
	//-----------Updateメンバ関数ポインタ---------
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);
	/// <summary>
	/// 変身中の演出
	/// </summary>
	/// <param name=""></param>
	void CopyingUpdate(Input&);
	/// <summary>
	/// 死ぬときの処理(アニメーション)
	/// </summary>
	/// <param name=""></param>
	void DyingUpdate(Input&);
	/// <summary>
	/// カメラが揺れるときは時を止める
	/// </summary>
	/// <param name=""></param>
	void ShakingUpdate(Input&);

	using UpdateFunc_t = void(GameScene1_3::*)(Input&);
	UpdateFunc_t update_;	// Update系を受け取るメンバ関数ポインタ
	//-------------------------------------------------------------
	//-----------------------Drawメンバ関数ポインタ------------------------

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(GameScene1_3::*)();
	DrawFunc_t draw_;	// Draw系を受け取るメンバ関数ポインタ


	//敵の位置情報のベクター(type,pos,spawned)
	std::vector<EnemySpawn> m_enemySpawns;
	//敵を生成するunique_ptrのベクター
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	//カメラの内側にいるかどうか
	int cameraframeMargin = 100;
	bool IsInCamera(float x, float y)
	{
		float left = camera.pos.x - screenWidth / 2 - cameraframeMargin;//???一度わからなくなった
		float right = camera.pos.x + screenWidth / 2 + cameraframeMargin;//???一度わからなくなった
		float top = camera.pos.y - cameraframeMargin;
		float bottom = camera.pos.y + screenHeight + cameraframeMargin;


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
	int m_frame = 0;	// フェードインアウト用
	int m_gameFrame = 0;	// ゲーム中のフレームをカウント
	int m_shakeTime = 0;//カメラが揺れてる間ほかを止める
	int m_hitCoolFrame = 0;//被弾後のクールタイム
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
	StageUI stageUI;//UI

};

