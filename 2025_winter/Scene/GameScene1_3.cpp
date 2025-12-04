#include "GameScene1_3.h"
#include <algorithm>
#include "DxLib.h"
#include "Camera.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "EnemyArrow.h"
#include "Arrow.h"
#include "Item.h"
#include "Frozen.h"
#include "BurningObject.h"
#include <../Door.h>
#include "Bg.h"
#include "../input.h"
#include "GameoverScene.h"
#include "GameClearScene.h"
#include "SceneController.h"
#include <cassert>
#include "../Application.h"


namespace
{

	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;

	constexpr int fade_interval = 60;

	constexpr int shake_interval = 30;

	constexpr float FrozenSpeed = 13.0f;

}


GameScene1_3::GameScene1_3(SceneController& controller,PlayerType type,int hp) :
	Scene(controller),
	update_(&GameScene1_3::FadeInUpdate),
	draw_(&GameScene1_3::FadeDraw)

{
	//実質Initの使い方
	//m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Normal, Vec2(700.0f,600.0f), false });//移動用
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(1100.0f,500.0f), false });//上
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(1500.0f,650.0f), false });//下
	m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Walk, Vec2(1700.0f,400.0f), false });
	m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(1700.0f,390.0f), false });
	m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Normal, Vec2(2200.0f,600.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(2700.0f,600.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2550.0f,810.0f), false });//崖下
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2800.0f,800.0f), false });//崖下
	m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(3150.0f,630.0f), false });//崖の上
	m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Walk, Vec2(4350.0f,690.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(4865.0f,590.0f), false });
	m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(5315.0f,860.0f), false });
	m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Normal, Vec2(5784.0f,520.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(6500.0f,520.0f), false });//崖上三連単
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(6586.0f,520.0f), false });//崖上三連単
	m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(6600.0f,520.0f), false });//崖上三連単

	InitCamera(camera);//カメラの初期化
	//-----------------------------------------------------------------
	m_frame = fade_interval;// フェードインの最初
	m_pPlayer = std::make_shared<Player>(type,hp);
	m_pBg = new Bg(m_pPlayer, 3);
	m_doors = std::make_shared< Door>(Vec2{ 6000,850 });

	//シーン切り替え後のにゅいーんをなくす
	stageUI.Init(hp);



	//後で張り付ける
		////敵の情報を送る
		//arrow->SetEnemyWizard(m_pEnemyWizards);
		//arrow->SetPlayer(m_pPlayer);



	m_pPlayer->SetBgPointer(m_pBg);
	//m_pItems->SetBgPointer(m_pBg);
}


void GameScene1_3::CheckHit()
{
	switch (m_pPlayer->GetType())
	{
	case PlayerType::Normal:
		CheckHitNormal();
		break;
	case PlayerType::Burning:
		CheckHitBurning();
		break;
	case PlayerType::Frozen:
		CheckHitFrozen();
		break;
	}
	if (m_pItems)
	{
		//プレイヤーがコピー状態かつ変身アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Copy)
		{
			if (m_pPlayer->GetAnimIdx() > 3)
			{
				//プレイヤーの当たり判定とアイテムの当たり判定をチェック
				bool isHitItem = m_pPlayer->GetColCopyRect().IsCollision(m_pItems->GetColRect());
				if (isHitItem)
				{
					//ここにアイテムを取得したときの処理を書く
					if (m_pItems->GetItemState() == ItemState::Burning)//バーニングのアイテムでの変身
					{

						m_pPlayer->ChangeBurning();
						m_pItems = nullptr;

					}
					else if (m_pItems->GetItemState() == ItemState::Frozen)//フローズンのアイテムでの変身
					{

						m_pPlayer->ChangeFrozen();
						m_pItems = nullptr;

					}
					else if (m_pItems->GetItemState() == ItemState::Archer)//アーチャーのアイテムでの変身
					{

						m_pPlayer->ChangeArcher();
						m_pItems = nullptr;

					}
				}
			}

		}

	}

}



void GameScene1_3::CheckArrowHit()
{
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyWizard)continue;

		std::shared_ptr<EnemyWizard> enemy = num->hitEnemyWizard;

		//敵リストから一致するやつを探して削除
		for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)
		{
			auto& e = m_pEnemyWizards[i];
			if (e == enemy)
			{

				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
				break;
			}
		}
		//矢のヒット情報をリセット
		num->hitEnemyWizard = nullptr;
	}
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyRider)continue;

		std::shared_ptr<EnemyRider> enemy = num->hitEnemyRider;

		//敵リストから一致するやつを探して削除
		for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)
		{
			auto& e = m_pEnemyRiders[i];
			if (e == enemy)
			{

				//消えるとき絶対する処理
		//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
				break;
			}
		}
		//矢のヒット情報をリセット
		num->hitEnemyRider = nullptr;
	}
	for (auto& num : m_arrows)//どくろアーチャー
	{
		if (num == nullptr || !num->hitEnemyArcher)continue;

		std::shared_ptr<EnemyArcher> enemy = num->hitEnemyArcher;

		//敵リストから一致するやつを探して削除
		for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)
		{
			auto& e = m_pEnemyArchers[i];
			if (e == enemy)
			{

				//消えるとき絶対する処理
		//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
				break;
			}
		}
		//矢のヒット情報をリセット
		num->hitEnemyArcher = nullptr;
	}

	//敵の矢の情報を書く(プレイヤーと当たった時)
	for (auto& e_arrow : m_pEnemyArrows)
	{
		if (e_arrow == nullptr || !e_arrow->m_hitPlayer)continue;

		bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(e_arrow->GetColRect());
		//敵がどっちから当たったかどうかを入れる
		//矢とどの方向で当たったかどうか
		m_pPlayer->DamageHit(isLeft);
		OnShake();
		e_arrow->m_hitPlayer = nullptr;

	}
}

void GameScene1_3::CheckFrozenHit()
{
	for (auto& m_pFrozen : m_pFrozens)
	{
		if (!m_pFrozen)continue;
		//プレイヤーと当たった時の反応
		bool isHitFrozen = m_pPlayer->GetColRect().IsCollision(m_pFrozen->GetColRect());
		if (isHitFrozen && !(m_pFrozen->isMove))
		{
			//プレイヤーが攻撃モーションのときは氷は動かせない
			if (m_pPlayer->GetState() == PlayerState::Attack)return;
			bool dir = m_pPlayer->GetPos().x < m_pFrozen->GetPos().x;
			Vec2 add = { dir ? FrozenSpeed : -FrozenSpeed,   0 };
			m_pFrozen->AddVel(add);

			m_pFrozen->isMove = true;
		}
		if (m_pFrozen->isMove)
		{
			//動いているときに敵と当たる//ペンギン
			for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)
			{
				auto& e = m_pEnemyWizards[i];
				if (!e)continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = e->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					m_pFrozen = nullptr;
					//消えるとき絶対する処理
				//対応するspawnを復活可能にする
					EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
					spawn.spawned = false;
					spawn.wasKilled = true;

					//インスタンスを消す
					m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
				}

			}
			//動いているときに敵と当たる//オークライダー
			for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)
			{
				auto& e = m_pEnemyRiders[i];
				if (!e)continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = e->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					m_pFrozen = nullptr;

					//消えるとき絶対する処理
			//対応するspawnを復活可能にする
					EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
					spawn.spawned = false;
					spawn.wasKilled = true;

					//インスタンスを消す
					m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);

				}

			}
			//動いているときに敵と当たる//どくろアーチャー
			for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)
			{
				auto& e = m_pEnemyArchers[i];
				if (e == nullptr)continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = e->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					m_pFrozen = nullptr;
					//消えるとき絶対する処理
				//対応するspawnを復活可能にする
					EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
					spawn.spawned = false;
					spawn.wasKilled = true;

					//インスタンスを消す
					m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);


				}

			}
		}

		//ついでにカメラの外に出たら消す処理
		if (!m_pFrozen) return;
		//カメラの外だったらnullptrにする
		float posX = m_pFrozen->GetPos().x + camera.drawOffset.x;
		float posY = m_pFrozen->GetPos().y + camera.drawOffset.y;

		if (posX < 0 || posX > kScreenWidth)
		{
			m_pFrozen = nullptr;
		}

		if (m_pFrozen == nullptr) continue;
		if (posY < 0 || posY > kScreenHeight)
		{
			m_pFrozen = nullptr;
		}

	}


}

void GameScene1_3::ReactionBurning()
{
	for (auto& m_pBurningObject : m_pBurningObjects)
	{
		if (!m_pBurningObject) continue;
		//カメラの外だったらnullptrにする
		float posX = m_pBurningObject->GetPos().x + camera.drawOffset.x;
		float posY = m_pBurningObject->GetPos().y + camera.drawOffset.y;

		if (posX < 0 || posX > kScreenWidth)
		{
			m_pBurningObject = nullptr;
		}

		if (m_pBurningObject == nullptr) continue;
		if (posY < 0 || posY > kScreenHeight)
		{
			m_pBurningObject = nullptr;
		}

	}


}

void GameScene1_3::CheckHitNormal()
{

	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{

		auto& e = m_pEnemyWizards[i];
		if (e == nullptr)continue;
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(e->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//e->DropItem(e);
				//アイテムを落とす処理
				m_pItems = std::make_shared<Item>(e);//新しくアイテムを生成
				m_pItems->SetBgPointer(m_pBg);
				m_pItems->ChangePos() = e->GetPos();
				////敵のヒット情報をリセット
				//wizard->HitWizard = nullptr;
				//ここに敵が攻撃されたときの処理を書く
				//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);

			}
		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pEnemyRiders[i];
			if (e == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(e->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//e->DropItem(e);
				//ここに敵が攻撃されたときの処理を書く
				//if (e == nullptr || !rider->HitRider)continue;

				//アイテムを落とす処理
				m_pItems = std::make_shared<Item>(e);//新しくアイテムを生成//別のアイテムを渡す
				m_pItems->SetBgPointer(m_pBg);
				m_pItems->ChangePos() = e->GetPos();
				//敵のヒット情報をリセット
				//num->HitRider = nullptr;

					////敵のヒット情報をリセット
				//wizard->HitWizard = nullptr;
				//ここに敵が攻撃されたときの処理を書く
				//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);

			}

		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pEnemyArchers[i];
			if (e == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(e->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//num->DropItem(num);
				//ここに敵が攻撃されたときの処理を書く
				//if (num == nullptr || !rider->HitRider)continue;

				//アイテムを落とす処理
				m_pItems = std::make_shared<Item>(e);//新しくアイテムを生成//別のアイテムを渡す
				m_pItems->SetBgPointer(m_pBg);
				m_pItems->ChangePos() = e->GetPos();
				//敵のヒット情報をリセット
				//num->HitRider = nullptr;

			//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);


			}

		}
	}
}

void GameScene1_3::CheckHitBurning()
{
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack)// && m_pPlayer->GetAnimIdx() >= 0
		{
			auto& e = m_pEnemyWizards[i];
			if (!e)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(e->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));//演出の炎をpush_back
				//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
			}

		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			auto& e = m_pEnemyRiders[i];
			if (!e)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(e->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));
				//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
			}

		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			auto& e = m_pEnemyArchers[i];
			if (e == nullptr)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(e->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
			}

		}
	}
}

void GameScene1_3::CheckFastBurning()
{
	if (!m_pPlayer) return;
	m_pPlayer->ChangeBurningAttack(false);
	Vec2 p0 = m_pPlayer->GetPrevPos();
	Vec2 p1 = m_pPlayer->GetAfterPos();
	//敵のやられ判定の処理
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		auto& e = m_pEnemyWizards[i];
		if (!e)continue;
		//バーニングの攻撃の矩形との当たり判定チェック
		if (CheckSweepHit(p0, p1, e->GetColRect()))
		{
			//ここに敵が攻撃されたときの処理を書く
			m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));//演出の炎をpush_back
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//オークライダー
	{
		auto& e = m_pEnemyRiders[i];
		if (!e)continue;
		//バーニングの攻撃の矩形との当たり判定チェック
		if (CheckSweepHit(p0, p1, e->GetColRect()))
		{
			//ここに敵が攻撃されたときの処理を書く
			m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));//演出の炎をpush_back
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//オークライダー
	{
		auto& e = m_pEnemyArchers[i];
		if (!e)continue;
		//バーニングの攻撃の矩形との当たり判定チェック
		if (CheckSweepHit(p0, p1, e->GetColRect()))
		{
			//ここに敵が攻撃されたときの処理を書く
			m_pBurningObjects.push_back(std::make_shared<BurningObject>(e));//演出の炎をpush_back
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
		}
	}

}

void GameScene1_3::CheckHitFrozen()
{
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pEnemyWizards[i];
			if (!e)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(e->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(e));
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
			}

		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pEnemyRiders[i];
			if (!e)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(e->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(e));
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
			}

		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pEnemyArchers[i];
			if (e == nullptr)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(e->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(e));
				//消えるとき絶対する処理
				//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
			}

		}
	}
}

void GameScene1_3::CheckPlayer()
{
	if (!m_pPlayer)return;
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		auto& e = m_pEnemyWizards[i];
		if (!e)continue;
		//バーニングの攻撃中は無敵
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetType() == PlayerType::Burning)
			continue;

		//プレイヤーの当たり判定が敵の当たり判定と当たった時
		if (m_pPlayer->GetColRect().IsCollision(e->GetColRect()))
		{
			bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(e->GetColRect());
			//敵がどっちから当たったかどうかを入れる
			//プレイヤーのダメージ処理
			m_pPlayer->DamageHit(isLeft);
			OnShake();
			//敵が消える処理
			//消えるとき絶対する処理
				//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);

		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//オークライダー
	{
		auto& e = m_pEnemyRiders[i];
		if (!e)continue;

		//バーニングの攻撃中は無敵
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetType() == PlayerType::Burning)
			continue;
		//プレイヤーの当たり判定が敵の当たり判定と当たった時
		if (m_pPlayer->GetColRect().IsCollision(e->GetColRect()))
		{
			bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(e->GetColRect());
			//敵がどっちから当たったかどうかを入れる
			//プレイヤーのダメージ処理
			m_pPlayer->DamageHit(isLeft);
			OnShake();
			//敵が消える処理
			//消えるとき絶対する処理
				//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);

		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//どくろアーチャー
	{
		auto& e = m_pEnemyArchers[i];
		if (!e)continue;

		//バーニングの攻撃中は無敵
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetType() == PlayerType::Burning)
			continue;

		//プレイヤーの当たり判定が敵の当たり判定と当たった時
		if (m_pPlayer->GetColRect().IsCollision(e->GetColRect()))
		{
			bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(e->GetColRect());
			//敵がどっちから当たったかどうかを入れる
			//プレイヤーのダメージ処理
			m_pPlayer->DamageHit(isLeft);
			OnShake();
			//敵が消える処理
			//消えるとき絶対する処理
				//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);

		}
	}


}
bool  GameScene1_3::CheckDropped()
{
	bool dropped = m_pPlayer->GetPos().y > screenHeight + 50;//UIの分上に上がったのでその分

	if (dropped)
	{
		//DyingActと同じ処理
		{
			m_pPlayer->Death();
			StartCameraShake(camera, 20.0f, 0.2f);
			update_ = &GameScene1_3::ShakingUpdate;

		}

		return true;
	}
	return false;
}
void GameScene1_3::DyingAct()
{
	m_pPlayer->Death();
	StartCameraShake(camera, 20.0f, 0.2f);
	update_ = &GameScene1_3::ShakingUpdate;

}
void GameScene1_3::OnShake()
{
	StartCameraShake(camera, 15.0f, 0.1f);
}

void GameScene1_3::FadeInUpdate(Input&)
{



	if (m_frame-- <= 0)
	{

		update_ = &GameScene1_3::NormalUpdate;
		draw_ = &GameScene1_3::NormalDraw;
		return;
	}
}

void GameScene1_3::NormalUpdate(Input& input)
{
	//----カメラの位置-----------------------------------
	float left = camera.pos.x - screenWidth / 2 + 10;
	float right = camera.pos.x + screenWidth / 2 - 10;
	float top = camera.pos.y - cameraMargin;
	float bottom = camera.pos.y + screenHeight + cameraMargin;
	//----------------------------------------------------

	//復活チェック
	for (auto& spawn : m_enemySpawns)
	{
		if (spawn.wasKilled)
		{
			bool leftout = spawn.pos.x < left;
			bool rightout = spawn.pos.x > right;
			if (leftout || rightout)
			{
				spawn.wasKilled = false;//復活可能
			}
		}

	}


	//生成
	for (auto& spawn : m_enemySpawns)//敵の生成する場所
	{
		if (!spawn.spawned && !spawn.wasKilled)//spawnされてなかったら//復活できる状態だったら
		{

			if (IsInCamera(spawn.pos.x, spawn.pos.y))//カメラの中にいたら
			{
				spawn.spawned = true;

				if (spawn.type == EnemyType::Wizard)
				{
					auto enemy = std::make_shared<EnemyWizard>();
					enemy->SetBgPointer(m_pBg);

					enemy->SetPlayer(m_pPlayer);
					enemy->SetState(spawn.state);
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyWizards.push_back(enemy);
				}
				else if (spawn.type == EnemyType::Rider)
				{
					auto enemy = std::make_shared<EnemyRider>();
					enemy->SetBgPointer(m_pBg);
					enemy->SetPlayer(m_pPlayer);
					enemy->SetState(spawn.state);
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyRiders.push_back(enemy);
				}
				else if (spawn.type == EnemyType::Archer)
				{
					auto enemy = std::make_shared<EnemyArcher>();
					enemy->SetBgPointer(m_pBg);
					enemy->SetPlayer(m_pPlayer);
					enemy->SetState(spawn.state);
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyArchers.push_back(enemy);

				}
			}

		}
	}

	//プレイヤーのHPを引き渡す
	stageUI.SetHp(m_pPlayer->GetHp());
	//プレイヤーのTypeを引き渡す
	stageUI.SetType(m_pPlayer->GetType());
	stageUI.Update();
	//プレイヤーのHpが0以下だったら死ぬ
	if (m_pPlayer->GetHp() <= 0)
	{
		DyingAct();
		return;
	}



	const auto& wsize = Application::GetInstance().GetWindowSize();
	//プレイヤーが落ちたかどうか
	if (CheckDropped())return;//落ちてたらそのあとの処理をしない

	///*if (input.IsTriggered("ok"))
	//{
	//	update_ = &GameScene1_3::FadeOutUpdate;
	//	draw_ = &GameScene1_3::FadeDraw;
	//}*/
	//ドアに触れているかつ上入力をしていたらシーン遷移
	if (m_pPlayer->GetColRect().IsCollision(m_doors->GetColRect()) && input.IsTriggered("up"))
	{
		update_ = &GameScene1_3::FadeOutUpdate;
		draw_ = &GameScene1_3::FadeDraw;
	}

	//ポーズ画面
	/*if (input.IsTriggered("pause"))
	{
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}*/
	UpdateCamera(camera, m_pPlayer);


	m_pBg->Update();
	m_doors->Update();

	m_pPlayer->Update(input);
	if (m_pPlayer->isBurningAttack)//バーニングの攻撃の出始めのほうの判定
	{
		CheckFastBurning();
	}

	if (m_pPlayer->isArrowAttack)//矢の出現
	{
		//ポインタを作ってその座標を入れる
		std::shared_ptr<Arrow> arrow = m_pPlayer->ShotArrow();

		//それをpush_backする
		m_arrows.push_back(arrow);

		m_pPlayer->isArrowAttack = false;
	}
	for (auto& m_pEnemyArcher : m_pEnemyArchers)//敵の矢の出現
	{
		if (!m_pEnemyArcher)continue;
		if (m_pEnemyArcher->isArrowAttack)
		{
			//ポインタを作ってその座標を入れる
			std::shared_ptr<EnemyArrow> arrow = m_pEnemyArcher->ShotArrow();

			//それをpush_backする
			m_pEnemyArrows.push_back(arrow);

			m_pEnemyArcher->isArrowAttack = false;
		}

	}


	for (auto& enemy : m_pEnemyWizards)//ペンギン
	{
		if (enemy) enemy->Update();
	}

	for (auto& enemy : m_pEnemyRiders)//オークライダー
	{
		if (enemy) enemy->Update();
	}
	for (auto& enemy : m_pEnemyArchers)//狙撃手
	{
		if (enemy) enemy->Update();
	}

	if (m_pItems) m_pItems->Update();
	for (auto& arrow : m_arrows)//矢のアップデート
	{

		if (!arrow) continue;
		if (arrow->isAlive == false)//矢が消えてるサインが出たらけす
		{
			arrow = nullptr;
			continue;
		}
		//敵を渡すようにする
		arrow->Update();
		arrow->CheckEnemys(m_pEnemyWizards);
		arrow->CheckEnemys(m_pEnemyRiders);
		arrow->CheckEnemys(m_pEnemyArchers);
	}
	for (auto& arrow : m_pEnemyArrows)//敵の矢のアップデート
	{

		if (!arrow) continue;
		if (arrow->isAlive == false)//矢が消えてるサインが出たらけす
		{
			arrow = nullptr;
			continue;
		}
		//敵を渡すようにする
		arrow->Update();
		arrow->CheckPlayer(m_pPlayer);

	}
	for (auto& m_pFrozen : m_pFrozens)
	{
		if (m_pFrozen) m_pFrozen->Update();
	}
	for (auto& m_pBurningObject : m_pBurningObjects)
	{
		if (m_pBurningObject) m_pBurningObject->Update();
	}
	CheckHit();//3種の攻撃の当たり判定
	CheckArrowHit();
	//CheckItemWizard();
	//CheckItemOrcRider();
	CheckFrozenHit();
	CheckPlayer();

	//消える処理
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		if (!m_pEnemyWizards[i])continue;
		auto& e = m_pEnemyWizards[i];
		if (!IsInCamera(e->GetPos().x, e->GetPos().y))
		{
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
		}
	}
	for (int i = (int)m_pEnemyRiders.size() - 1; i >= 0; i--)//ライダー
	{
		if (!m_pEnemyRiders[i])continue;
		auto& e = m_pEnemyRiders[i];
		if (!IsInCamera(e->GetPos().x, e->GetPos().y))
		{
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
		}
	}
	for (int i = (int)m_pEnemyArchers.size() - 1; i >= 0; i--)//どくろアーチャー
	{
		if (!m_pEnemyArchers[i])continue;
		auto& e = m_pEnemyArchers[i];
		if (!IsInCamera(e->GetPos().x, e->GetPos().y))
		{
			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
		}
	}




}

void GameScene1_3::FadeOutUpdate(Input&)
{
	//ドアの描画(FadeOutDrawがないため,
	// いったんこっちにおく)
	m_doors->OutUpdate();
	m_pBg->Draw(camera);
	m_doors->Draw(camera);
	stageUI.Draw(camera);
	m_pPlayer->Draw(camera);

	if (m_frame++ >= fade_interval)
	{
		//delete m_pCharacter;
		delete m_pBg;
		controller_.ChangeScene(std::make_shared<GameClearScene>(controller_));
		return;
	}
}
void GameScene1_3::DyingUpdate(Input& input)
{
	
	m_pBg->Draw(camera);
	stageUI.Draw(camera);
	m_pPlayer->DyingUpdate();
	m_pPlayer->DyingDraw(camera);

	if (m_frame++ >= fade_interval * 1.5f)
	{
		//delete m_pCharacter;
		delete m_pBg;
		controller_.ChangeScene(std::make_shared<GameScene1_3>(controller_, PlayerType::Normal,100));
		return;
	}
}
void GameScene1_3::ShakingUpdate(Input&)
{
	UpdateCamera(camera, m_pPlayer);
	if (m_shakeTime++ >= shake_interval)
	{
		m_shakeTime = 0;
		update_ = &GameScene1_3::DyingUpdate;
		draw_ = &GameScene1_3::FadeDraw;
	}
}
void GameScene1_3::FadeDraw()
{


	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	//DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



}

void GameScene1_3::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	m_pBg->Draw(camera);
	m_doors->Draw(camera);
	

	for (auto& m_pFrozen : m_pFrozens)
	{
		if (m_pFrozen) m_pFrozen->Draw(camera);
	}
	m_pPlayer->Draw(camera);
	for (auto& enemy : m_pEnemyWizards)//ペンギン
	{
		if (enemy) enemy->Draw(camera);
	}
	for (auto& enemy : m_pEnemyRiders)//オークライダー
	{
		if (enemy) enemy->Draw(camera);
	}
	for (auto& enemy : m_pEnemyArchers)//狙撃手
	{
		if (enemy) enemy->Draw(camera);
	}
	if (m_pItems) m_pItems->Draw(camera);
	for (auto& arrow : m_arrows)//弓矢
	{
		if (!arrow) continue;
		arrow->Draw(camera);
	}
	for (auto& arrow : m_pEnemyArrows)//敵の矢
	{
		if (!arrow) continue;
		arrow->Draw(camera);
	}
	for (auto& m_pBurningObject : m_pBurningObjects)
	{
		if (m_pBurningObject) m_pBurningObject->Draw(camera);
	}
	ReactionBurning();
	//ステージUI
	stageUI.Draw(camera);

	float left = camera.pos.x + m_cameraMargin - screenWidth / 2;
	float right = camera.pos.x + screenWidth / 2 - m_cameraMargin;
	float top = camera.pos.y + 10;
	float bottom = camera.pos.y + screenHeight - 10;
	DrawBox(left + camera.drawOffset.x, top, right + camera.drawOffset.x, bottom, GetColor(255, 255, 0), false);

}

bool GameScene1_3::CheckSweepHit(const Vec2& p0, const Vec2& p1, const Rect& rect)
{
	// --- ① まず AABB でざっくり判定 ---
	float sweepLeft = (std::min)(p0.x, p1.x);
	float sweepRight = (std::max)(p0.x, p1.x);
	float sweepTop = (std::min)(p0.y, p1.y);
	float sweepBottom = (std::max)(p0.y, p1.y);

	// AABB が敵矩形とまったく重ならない → 交差してない
	if (sweepRight < rect.Getleft() || sweepLeft > rect.GetRight() ||
		sweepBottom < rect.GetTop() || sweepTop > rect.GetBottom())
	{
		return false;
	}
	// --- ② 線分と矩形の正確判定（Liang–Barsky）---

	float dx = p1.x - p0.x;
	float dy = p1.y - p0.y;

	float t0 = 0.0f, t1 = 1.0f;

	auto clip = [&](float p, float q) -> bool
		{
			if (p == 0.0f) {
				// 線が矩形と平行
				return (q >= 0.0f);
			}
			float t = q / p;
			if (p < 0.0f) {
				if (t > t1) return false;
				if (t > t0) t0 = t;
			}
			else {
				if (t < t0) return false;
				if (t < t1) t1 = t;
			}
			return true;
		};

	// 左（x >= r.left）
	if (!clip(-dx, p0.x - rect.Getleft()))   return false;
	// 右（x <= r.right）
	if (!clip(dx, rect.GetRight() - p0.x))  return false;
	// 上（y >= r.top）
	if (!clip(-dy, p0.y - rect.GetTop()))    return false;
	// 下（y <= r.bottom）
	if (!clip(dy, rect.GetBottom() - p0.y)) return false;

	// t0～t1 の間で交差が確認されたらヒット
	return true;
}

void GameScene1_3::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene1_3::Draw()
{
	(this->*draw_)();
}

