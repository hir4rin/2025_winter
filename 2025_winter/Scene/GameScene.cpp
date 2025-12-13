#include "GameScene.h"
#include <algorithm>
#include "DxLib.h"
#include "Camera.h"
#include "Effect.h"
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
#include "PauseScene.h"
#include "SceneController.h"
#include <cassert>
#include "../Application.h"


namespace
{

	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;

	constexpr int fade_interval = 75; 
	constexpr int copy_interval = 30; 

	constexpr int shake_interval = 30;
	constexpr int hit_interval = 40;


	constexpr float FrozenSpeed = 13.0f;

}


GameScene::GameScene(SceneController& controller, int stageNum,PlayerType type,int hp) :
	Scene(controller),
	m_stageNum(stageNum),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeInDraw)

{
	switch (m_stageNum)
	{
	case 1:
		//実質Initの使い方
	//m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Normal, Vec2(1100.0f,500.0f), false });//移動用
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(1500.0f,500.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2100.0f,700.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(3000.0f,500.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(4000.0f,200.0f), false });
		//-----------------------------------------------------------------


		m_pPlayer = std::make_shared<Player>(PlayerType::Normal, hp, Vec2{ 100,736 });

		m_pBg = new Bg(m_pPlayer, 1);
		m_doors = std::make_shared< Door>(Vec2{ 5200,660 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });


		break;
	case 2:
		//実質Initの使い方
	//m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Walk, Vec2(700.0f,600.0f), false });//移動用
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Walk, Vec2(1000.0f,600.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(2000.0f,200.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2600.0f,200.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(1884.0f,880.0f), false });//tika
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2400.0f,880.0f), false });//tika
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(3000.0f,250.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(3700.0f,200.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(4750.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(4000.0f,1000.0f), false });//地下
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(5100.0f,200.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(5600.0f,200.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(5650.0f,200.0f), false });
		//-----------------------------------------------------------------

		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 });

		m_pBg = new Bg(m_pPlayer, 2);
		//m_doors = std::make_shared< Door>(Vec2{ 400,850 });
		m_doors = std::make_shared< Door>(Vec2{ 6000,850 });
		break;
	case 3:
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


		//-----------------------------------------------------------------

		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 });
		m_pBg = new Bg(m_pPlayer, 3);
		m_doors = std::make_shared< Door>(Vec2{ 6000,850 });
		break;
	}
	
	m_frame = fade_interval;// フェードインの最初
	
	InitCamera(camera);//カメラの初期化

	//シーン切り替え後のにゅいーんをなくす
	stageUI.Init(hp);


	m_pPlayer->SetBgPointer(m_pBg);
	//m_pItems->SetBgPointer(m_pBg);

	//playerの状態によってエフェクトを出す
	m_pPlayer->AddOnLandEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "star"));
		});
	m_pPlayer->AddOnWalkEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "dust"));
		});
	m_pPlayer->AddOnDashEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "dust"));
		});

}




void GameScene::CheckHit()
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
	

}



void GameScene::CheckArrowHit()
{
	//for (auto& num : m_arrows)//壁に当たったら消す
	//	
	//{
	//	if (num == nullptr )continue;
	//	Rect m_arrowRect = num->GetColRect();
	//	Rect chipRect;

	//	if (m_pBg->IsCollision(m_arrowRect,chipRect))
	//	{
	//		num = nullptr;
	//		continue;
	//	}
	//}
	for (auto it = m_arrows.begin(); it != m_arrows.end(); )//壁に当たったら消す
	{
		if (*it == nullptr)
		{
			it = m_arrows.erase(it);
			continue;
		}

		Rect m_arrowRect = (*it)->GetColRect();
		Rect chipRect;

		if (m_pBg->IsCollision(m_arrowRect, chipRect))
		{
			it = m_arrows.erase(it);
			continue;
		}

		++it;
	}
	for (auto it = m_pEnemyArrows.begin(); it != m_pEnemyArrows.end(); )//壁に当たったら消す(敵の矢も)
	{
		if (*it == nullptr)
		{
			it = m_pEnemyArrows.erase(it);
			continue;
		}

		Rect m_arrowRect = (*it)->GetColRect();
		Rect chipRect;

		if (m_pBg->IsCollision(m_arrowRect, chipRect))
		{
			it = m_pEnemyArrows.erase(it);
			continue;
		}

		++it;
	}
	
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

void GameScene::CheckFrozenHit()
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
		else if (m_pFrozen->isMove == false)
		{
			//動いていないときに敵と当たる//ペンギン
			for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)
			{
				auto& e = m_pEnemyWizards[i];
				if (!e)continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = e->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					//X軸の速度を反転する
					//キャラの向きを変える
					e->ChangeVel().x *= -1;
					e->Changem_isRight();
				
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
					//X軸の速度を反転する
					//キャラの向きを変える
					e->ChangeVel().x *= -1;
					e->Changem_isRight();
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
					//X軸の速度を反転する
					//キャラの向きを変える
					e->ChangeVel().x *= -1;
					e->Changem_isRight();
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

void GameScene::ReactionBurning()
{
	for (auto m_pBurningObject = m_pBurningObjects.begin(); m_pBurningObject != m_pBurningObjects.end();)
	{
		auto& obj = *m_pBurningObject;
		if (!obj)
		{
			m_pBurningObject = m_pBurningObjects.erase(m_pBurningObject);
			continue;
		}
		/*if (!m_pBurningObject) continue;*/
		//カメラの外だったらけす+UI
		float posX = obj->GetPos().x + camera.drawOffset.x;
		float posY = obj->GetPos().y + camera.drawOffset.y;
		//画面外なら削除
		if (posX < 0 || posX > kScreenWidth || posY < 0 || posY > kScreenHeight)
		{
			m_pBurningObject = m_pBurningObjects.erase(m_pBurningObject);
			//m_pBurningObject = nullptr;
		}
		else
			{
			++m_pBurningObject;//残す//次の要素へ進む
		}

		//if (posY < 0 || posY > kScreenHeight)
		//{
		//	//m_pBurningObjects.erase(m_pBurningObject);
		//	m_pBurningObject = nullptr;
		//}

	}


}

void GameScene::CheckHitNormal()
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
				
				//エフェクトを出す
				m_pEffects.push_back(std::make_shared<Effect>(e->GetPos(), "blueStarLight"));

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

void GameScene::CheckHitBurning()
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

void GameScene::CheckFastBurning()
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

void GameScene::CheckHitFrozen()
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

void GameScene::CheckPlayer()
{

	if (!m_pPlayer)return;
	m_hitCoolFrame--;


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
			if (m_hitCoolFrame > 0)
			{
				//敵だけ消す
				//敵が消える処理
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyWizards.erase(m_pEnemyWizards.begin() + i);
			}
			else
			{
				m_hitCoolFrame = hit_interval; //無敵時間を設定

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
			if (m_hitCoolFrame > 0)
			{
				//敵が消える処理
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyRiders.erase(m_pEnemyRiders.begin() + i);
			}
			else
			{
				m_hitCoolFrame = hit_interval; //無敵時間を設定
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
			if (m_hitCoolFrame > 0)
			{
				//敵が消える処理
				//消えるとき絶対する処理
					//対応するspawnを復活可能にする
				EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
				spawn.spawned = false;
				spawn.wasKilled = true;

				//インスタンスを消す
				m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
			}
			else
			{
				m_hitCoolFrame = hit_interval; //無敵時間を設定
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

	
}



bool  GameScene::CheckDropped()
{
	bool dropped = m_pPlayer->GetPos().y > screenHeight + 50;//UIの分上に上がったのでその分

	
	if (dropped)
	{
		//DyingActと同じ処理
		{
			m_pPlayer->Death();
			stageUI.Init(0);//HPを0にする
			StartCameraShake(camera, 20.0f, 0.2f);
			update_ = &GameScene::ShakingUpdate;
			
		}
		
		return true; 
	}
	return false;
}

void GameScene::DyingAct()
{
	m_pPlayer->Death();
	StartCameraShake(camera, 20.0f, 0.2f);
	update_ = &GameScene::ShakingUpdate;

	
}

void GameScene::OnShake()
{
	StartCameraShake(camera, 15.0f, 0.1f);
}

void GameScene::CopyAct(Input& input)
{
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
						//ここでアップデートを変える
						update_ = &GameScene::CopyingUpdate;
						draw_ = &GameScene::NormalDraw;
						m_frame = 0;
						return;

					}
					else if (m_pItems->GetItemState() == ItemState::Frozen)//フローズンのアイテムでの変身
					{

						m_pPlayer->ChangeFrozen();
						m_pItems = nullptr;
						//ここでアップデートを変える
						update_ = &GameScene::CopyingUpdate;
						draw_ = &GameScene::NormalDraw;
						m_frame = 0;
						return;
					}
					else if (m_pItems->GetItemState() == ItemState::Archer)//アーチャーのアイテムでの変身
					{

						m_pPlayer->ChangeArcher();
						m_pItems = nullptr;
						//ここでアップデートを変える
						update_ = &GameScene::CopyingUpdate;
						draw_ = &GameScene::NormalDraw;
						m_frame = 0;
						return;
					}
				}
			}

		}

	}

	if (input.IsTriggered("CopyOut"))
	{
		if (!(m_pPlayer->GetType() == PlayerType::Normal))
		{
		
			//プレイヤーのタイプに応じて落とすアイテムを変える
			switch (m_pPlayer->GetType())
			{
			case PlayerType::Frozen:
				m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyWizard>());
				m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
				m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
				break;
			case PlayerType::Burning:
				m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyRider>());
				m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
				m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
				break;
			case PlayerType::Archer:
				m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyArcher>());
				m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
				m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
				break;
			}
			//プレイヤーをノーマルに戻す
			m_pPlayer->ChangeNormal();
			
		}
	}
}

void GameScene::FadeInUpdate(Input&)
{
	m_pPlayer->AnimFrameUpdate();
	if (m_pPlayer != nullptr)UpdateCamera(camera, m_pPlayer);


	

	if (m_frame-- <= 0)
	{

		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	//復活、生成チェック
	CheckSpawns();



	//お試しテスト用
	if (input.IsTriggered("ok"))
	{
		
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

#ifdef _DEBUG
	if (input.IsTriggered("ok"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeOutDraw;
	}
#endif
	//ドアに触れているかつ上入力をしていたらシーン遷移
	if (m_pPlayer->GetColRect().IsCollision(m_doors->GetColRect()) && input.IsTriggered("up"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeOutDraw;
		m_frame = 0;
	}

	//ポーズ画面
	if (input.IsTriggered("pause"))
	{
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}
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
	//敵関連
	{
	


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
		for (auto& pEnemyArcher : m_pEnemyArchers)//敵の矢の出現
		{
			if (!pEnemyArcher)continue;
			if (pEnemyArcher->isArrowAttack)
			{
				//ポインタを作ってその座標を入れる
				std::shared_ptr<EnemyArrow> arrow = pEnemyArcher->ShotArrow();

				//それをpush_backする
				m_pEnemyArrows.push_back(arrow);

				pEnemyArcher->isArrowAttack = false;
			}

		}
	}
	//矢関連
	{
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
	}

	//氷、炎オブジェクト
	{
		for (auto& pFrozen : m_pFrozens)
		{
			if (pFrozen) pFrozen->Update();
		}
		for (auto& pBurningObject : m_pBurningObjects)
		{
			if (pBurningObject) pBurningObject->Update();
		}
	}
	
	
	for (auto effect = m_pEffects.begin(); effect != m_pEffects.end(); )
	{
		(*effect)->Update();

		if ((*effect)->IsDead())
			effect = m_pEffects.erase(effect);  // ← 安全に削除
		else
			++effect;
	}
	if (m_pItems) m_pItems->Update();
	if (m_pDroppedItem)//演出のアイテムのアップデート
	{
		m_pDroppedItem->DroppedUpdate();
		if (m_pDroppedItem->IsDead())
		{
			m_pDroppedItem = nullptr;
		}
	}
	CheckHit();//3種の攻撃の当たり判定
	CopyAct(input);//アイテム取得の処理関連
	CheckArrowHit();
	CheckFrozenHit();
	CheckPlayer();

	CheckOutCamera();



}

void GameScene::FadeOutUpdate(Input&)
{



	
	m_doors->OutUpdate();
	
	


	if (m_frame++ >= fade_interval)
	{
		//delete m_pCharacter;
		delete m_pBg;
		switch (m_stageNum)
		{
		case 1:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum+1, m_pPlayer->GetType(), m_pPlayer->GetHp()));
			return;
			break;
		case 2:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum + 1, m_pPlayer->GetType(), m_pPlayer->GetHp()));
			return;
			break;
		case 3:
			controller_.ChangeScene(std::make_shared<GameClearScene>(controller_, m_pPlayer->GetType()));
			return;
			break;
		}


		
	}
}

void GameScene::CopyingUpdate(Input&)
{
	//変身中の処理
	m_pPlayer->AnimFrameUpdate();
	m_pPlayer->AnimFrameUpdate();
	if (m_frame++ >= copy_interval)
	{
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::DyingUpdate(Input& input)
{
	
	m_pPlayer->DyingUpdate();
	
	
	if (m_frame++ >= fade_interval * 1.5f)
	{
		//delete m_pCharacter;
		delete m_pBg;
		switch (m_stageNum)
		{
		case 1:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100));
			return;
			break;
		case 2:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100));
			return;
			break;
		case 3:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100));
			return;
			break;
		}




	
	}
}

void GameScene::ShakingUpdate(Input&)
{
	UpdateCamera(camera, m_pPlayer);

	if (m_shakeTime++ >= shake_interval)
	{
		m_shakeTime = 0;
		update_ = &GameScene::DyingUpdate;
		draw_ = &GameScene::FadeDraw;
	}
}

void GameScene::FadeDraw()
{
	


	NormalDraw();
	if(update_ == &GameScene::DyingUpdate)m_pPlayer->DyingDraw(camera);

	//フェード
	m_pBg->FadeInBg(camera);

	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	//DrawBox(0, 0, screenWidth, screenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



}

void GameScene::FadeInDraw()
{
	NormalDraw();
	

	//フェード
	m_pBg->FadeInBg(camera);

	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	//DrawBox(0, 0, screenWidth, screenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::FadeOutDraw()
{
	//フェードアウト用の処理
	NormalDraw();

	//フェード
	m_pBg->FadeOutBg(camera);

	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	//DrawBox(0, 0, screenWidth, screenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void GameScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	
	
	if (update_ == &GameScene::CopyingUpdate)//変身中の暗転
	{
		//灰色にしたいものをここに
		m_pBg->Draw(camera);
		m_doors->Draw(camera);
		
		if (m_pItems) m_pItems->Draw(camera);
		if (m_pItems) m_pDroppedItem->DroppedDraw(camera);
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
		for (auto& effect : m_pEffects)
		{
			if (effect) effect->Draw(camera);
		}
		for (auto& m_pFrozen : m_pFrozens)
		{
			if (m_pFrozen) m_pFrozen->Draw(camera);
		}

		// 2. 灰色オーバーレイ
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160); // 好きな濃さ
		DrawBox(0, 0, screenWidth, screenHeight, 0x202020, true);//下より濃いめ
		//DrawBox(0, 0, screenWidth, screenHeight, 0x303030, true);//
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//灰色にしたくないものをこっちに
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
		//ステージUI
		stageUI.Draw(camera);

		float left = camera.pos.x - screenWidth / 2 - cameraframeMargin;
		float right = camera.pos.x + screenWidth / 2 + cameraframeMargin;
		float top = camera.pos.y - cameraframeMargin;
		float bottom = camera.pos.y + screenHeight + cameraframeMargin;
		DrawBox(left + camera.drawOffset.x, top, right + camera.drawOffset.x, bottom, GetColor(255, 255, 0), false);
	}
	else
	{
		m_pBg->Draw(camera);
		m_doors->Draw(camera);
		for (auto& m_pFrozen : m_pFrozens)
		{
			if (m_pFrozen) m_pFrozen->Draw(camera);
		}
		if (update_ != &GameScene::DyingUpdate) m_pPlayer->Draw(camera);//死に際の描画は別
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
		if (m_pDroppedItem) m_pDroppedItem->DroppedDraw(camera);
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
		for (auto& effect : m_pEffects)
		{
			if (effect) effect->Draw(camera);
		}

		//ステージUI
		stageUI.Draw(camera);

		float left = camera.pos.x - screenWidth / 2 - cameraframeMargin;
		float right = camera.pos.x + screenWidth / 2 + cameraframeMargin;
		float top = camera.pos.y - cameraframeMargin;
		float bottom = camera.pos.y + screenHeight + cameraframeMargin;
		DrawBox(left + camera.drawOffset.x, top, right + camera.drawOffset.x, bottom, GetColor(255, 255, 0), false);
	}

	
}

bool GameScene::CheckSweepHit(const Vec2& p0, const Vec2& p1, const Rect& rect)
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

void GameScene::CheckSpawns()
{
	//----カメラの位置-----------------------------------
	float left = camera.pos.x - screenWidth / 2 - cameraframeMargin;
	float right = camera.pos.x + screenWidth / 2 + cameraframeMargin;
	float top = camera.pos.y - cameraframeMargin;
	float bottom = camera.pos.y + screenHeight + cameraframeMargin;
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

}

void GameScene::CheckOutCamera()
{
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

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
