#include "GameScene.h"
#include <algorithm>
#include "WaveManager.h"
#include "Wave.h"
#include "DxLib.h"
#include "Camera.h"
#include "Effect.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "EnemyEliteOrc.h"
#include "EnemyBear.h"
#include "EnemyWolf.h"
#include "Salmon.h"
#include "Fish.h"
#include "EnemyArrow.h"
#include "BossShot.h"
#include "Arrow.h"
#include "Item.h"
#include "Potion.h"
#include "Frozen.h"
#include "BurningObject.h"
#include <../Game/Door.h>
#include "Bg.h"
#include "../input.h"
#include "GameoverScene.h"
#include "GameClearScene.h"
#include "StageSelectScene.h"
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


GameScene::GameScene(SceneController& controller, int stageNum,PlayerType type,int hp,int Life) :
	Scene(controller),
	m_stageNum(stageNum),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeInDraw)

{
	switch (m_stageNum)
	{
	case 1://ステージ1_1
		//実質Initの使い方
	//m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Normal, Vec2(1100.0f,500.0f), false });//移動用
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(1500.0f,500.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2100.0f,700.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(3000.0f,500.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(4000.0f,200.0f), false });
		//-----------------------------------------------------------------


		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,736 },Life);

		m_pBg = new Bg(m_pPlayer, 1);
		m_doors = std::make_shared< Door>(Vec2{ 5200,660 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });


		break;
	case 2://ステージ1_2
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

		m_pPotions.push_back(std::make_shared<Potion>(Vec2(4566.0f, 992.0f)));


		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);

		m_pBg = new Bg(m_pPlayer, 2);
		//m_doors = std::make_shared< Door>(Vec2{ 400,850 });
		m_doors = std::make_shared< Door>(Vec2{ 6000,850 });
		break;
	case 3://ステージ1_3
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

		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);
		m_pBg = new Bg(m_pPlayer, 3);
		m_doors = std::make_shared< Door>(Vec2{ 6000,850 });
		break;
	case 6://ステージ2_1
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Walk, Vec2(1225.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(1850.0f,850.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(1850.0f,700.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(2800.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(3000.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Attack, Vec2(3280.0f,730.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(3700.0f,890.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(4164.0f,730.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Attack, Vec2(4420.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(4800.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(5400.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(5700.0f,900.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(6000.0f,800.0f), false });
		//敵スポーン
		
		//------------------------------------------------------------------
		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);

		m_pBg = new Bg(m_pPlayer, 6);
		m_doors = std::make_shared< Door>(Vec2{ 6050,760 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });
		break;
	case 7://ステージ2_2
		//敵スポーン
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Walk, Vec2(776.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(1100.0f,600.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Walk, Vec2(1550.0f,800.0f), false });//下の段
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(1560.0f,600.0f), false });//真ん中の段
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(1550.0f,350.0f), false });//上の段
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Normal, Vec2(2704.0f,730.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(3187.0f,600.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Walk, Vec2(4197.0f,800.0f), false });//下の段
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(4097.0f,600.0f), false });//中の段
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(4697.0f,400.0f), false });//上の段
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(5291.0f,800.0f), false });//下の段
		//------------------------------------------------------------------
		m_pPotions.push_back(std::make_shared<Potion>(Vec2(4707.0f, 800.0f)));

		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);

		m_pBg = new Bg(m_pPlayer, 7);
		m_doors = std::make_shared< Door>(Vec2{ 6130,600 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });
		break;
	case 8://ステージ2_3
		//敵スポーン
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(1300.0f,800.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Normal, Vec2(1422.0f,670.0f), false });
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Normal, Vec2(1544.0f,544.0f), false });
		m_pElite = std::make_shared<EnemyEliteOrc>();
		m_enemySpawns.push_back({ EnemyType::Archer,EnemyState::Normal, Vec2(3959.0f,600.0f), false });
		m_enemySpawns.push_back({ EnemyType::Rider,EnemyState::Attack, Vec2(4241.0f,670.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Walk, Vec2(4834.0f,670.0f), false });
		m_enemySpawns.push_back({ EnemyType::Wizard,EnemyState::Normal, Vec2(4952.0f,200.0f), false });

		//------------------------------------------------------------------
		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);

		
		m_pBg = new Bg(m_pPlayer, 8);

		m_pElite->SetPlayer(m_pPlayer);
		m_pElite->SetBgPointer(m_pBg);


		m_doors = std::make_shared< Door>(Vec2{ 4953,660 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });

		//波動
		m_pElite->AddOnAttackEndEvent([this]() {
			Vec2 startPos = m_pElite->GetPos();
			startPos.y += 16;//足元に合わせる

			//左方向
			m_waveManagers.push_back(std::make_unique<WaveManager>(startPos, -1)
			);

			//右方向
			m_waveManagers.push_back(std::make_unique<WaveManager>(startPos, 1)
			);

			//カメラを揺らす
			StartCameraShake(camera, 10.0f, 0.2f);
		});
		m_pElite->AddOnDeathEvent([this]() {
			//カメラを揺らす
			StartCameraShake(camera, 10.0f, 0.4f);
});


		break;
	case 9:
		//
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3000,800 }, 1));
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3300,800 }, 2));
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3450,800 }, 3));


		//
		/*m_pBear = std::make_shared<EnemyBear>();
		m_pWolf = std::make_shared<EnemyWolf>();*/

		m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 }, Life);


		m_pBg = new Bg(m_pPlayer, 8);
//		//熊
//		{
//			m_pBear->SetPlayer(m_pPlayer);
//			m_pBear->SetBgPointer(m_pBg);
//			//カメラ
//			m_pBear->AddOnAttack1EndEvent([this]() {
//				//カメラを揺らす
//				StartCameraShake(camera, 2.5f, 0.1f);
//});
//
//			//波動
//			m_pBear->AddOnAttackEndEvent([this]() {
//				Vec2 startPos = m_pBear->GetPos();
//				startPos.y += 16;//足元に合わせる
//
//				//左方向
//				m_waveManagers.push_back(std::make_unique<WaveManager>(startPos, -1)
//				);
//
//				//右方向
//				m_waveManagers.push_back(std::make_unique<WaveManager>(startPos, 1)
//				);
//
//				//カメラを揺らす
//				StartCameraShake(camera, 10.0f, 0.2f);
//			});
//			//鮭
//			m_pBear->AddOnAttack3EndEvent([this]() {
//				for (int i = 0; i < 3; i++)
//				{
//					m_pSalmons.push_back(std::make_shared<Salmon>(m_pBear->GetPos(), m_pBear->Getm_isRight(),i+1));
//			}
//			
//				//カメラを揺らす
//				StartCameraShake(camera, 10.0f, 0.2f);
//			});
//
//		}
//		//狼
//		{
//			m_pWolf->SetPlayer(m_pPlayer);
//			m_pWolf->SetBgPointer(m_pBg);
//		}


		//魚
		{
			for (auto& it : m_pFishers)
			{
				it->SetPlayer(m_pPlayer);
				it->SetBgPointer(m_pBg);
			}
		}
	

		m_doors = std::make_shared< Door>(Vec2{ 4953,660 });
		//m_doors = std::make_shared< Door>(Vec2{ 500,660 });
		break;
	}
	
	m_frame = fade_interval;// フェードインの最初
	
	InitCamera(camera);//カメラの初期化

	//シーン切り替え後のにゅいーんをなくす
	stageUI.Init(hp,m_pPlayer->GetType(),m_pPlayer->GetLife());


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

GameScene::~GameScene()
{

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
	for (auto it = m_pBossShots.begin(); it != m_pBossShots.end(); )//壁に当たったら消す(ボスの弾も)
	{
		if (*it == nullptr)
		{
			it = m_pBossShots.erase(it);
			continue;
		}

		Rect m_arrowRect = (*it)->GetColRect();
		Rect chipRect;

		if (m_pBg->IsCollision(m_arrowRect, chipRect))
		{
			it = m_pBossShots.erase(it);
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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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
				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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

	//矢とボスエリートが当たった時の処理
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyEliteOrc)continue;

		std::shared_ptr<EnemyEliteOrc> enemy = num->hitEnemyEliteOrc;

		//敵リストから一致するやつを探して削除
		
		auto& e = m_pElite;
			if (e == enemy)
			{

				
				//bossにダメージを与える
				e->HitBossDamage(10);
				break;
			}
		
		//矢のヒット情報をリセット
		num->hitEnemyEliteOrc = nullptr;
	}
	//矢とボス熊が当たったときの処理
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyBear)continue;

		std::shared_ptr<EnemyBear> enemy = num->hitEnemyBear;

		//敵リストから一致するやつを探して削除
		
		auto& e = m_pBear;
			if (e == enemy)
			{

				
				//bossにダメージを与える
				e->HitBossDamage(10);
				break;
			}
		
		//矢のヒット情報をリセット
		num->hitEnemyBear = nullptr;
	}
	//矢とボス狼が当たったときの処理
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyWolf)continue;

		std::shared_ptr<EnemyWolf> enemy = num->hitEnemyWolf;

		//敵リストから一致するやつを探して削除
		
		auto& e = m_pWolf;
			if (e == enemy)
			{

				
				//bossにダメージを与える
				e->HitBossDamage(10);
				break;
			}
		
		//矢のヒット情報をリセット
		num->hitEnemyWolf = nullptr;
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
	//ボスの弾の情報を書く(プレイヤーと当たった時)
	for (auto& e_shot : m_pBossShots)
	{
		if (e_shot == nullptr || !e_shot->m_hitPlayer)continue;

		bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(e_shot->GetColRect());
		//敵がどっちから当たったかどうかを入れる
		//矢とどの方向で当たったかどうか
		m_pPlayer->DamageHit(isLeft);
		OnShake();
		e_shot->m_hitPlayer = nullptr;

	}
	//サーモン
	for (auto& it : m_pSalmons)
	{
		if (it == nullptr)return;
		if (it->GetColRect().IsCollision(m_pPlayer->GetColRect()))
		{
			bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(it->GetColRect());
			//敵がどっちから当たったかどうかを入れる
			//矢とどの方向で当たったかどうか
			m_pPlayer->DamageHit(isLeft);
			OnShake();
			it = nullptr;
		}
	
	}

	
	//氷と当たったときの処理は氷の場所でやってる

	
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
			//動いているときに敵と当たる//ボスエリート
			{
				if (m_pElite != nullptr)
				{
					if (m_pFrozen)
					{
						bool isHitEnemy = m_pElite->GetColRect().IsCollision(m_pFrozen->GetColRect());

						if (isHitEnemy)
						{
							m_pFrozen = nullptr;
						

							//インスタンスを消す
							//ボスにダメージを与える
							m_pElite->HitBossDamage(20);


						}
					}
				
			    }

			}
			//動いているときに敵と当たる//ボス熊
			{
				if (m_pBear != nullptr)
				{
					if (m_pFrozen)
					{
						bool isHitEnemy = m_pBear->GetColRect().IsCollision(m_pFrozen->GetColRect());

						if (isHitEnemy)
						{
							m_pFrozen = nullptr;
						

							//インスタンスを消す
							//ボスにダメージを与える
							m_pBear->HitBossDamage(20);


						}
					}
				
			    }

			}
			//動いているときに敵と当たる//ボス狼
			{
				if (m_pWolf != nullptr)
				{
					if (m_pFrozen)
					{
						bool isHitEnemy = m_pWolf->GetColRect().IsCollision(m_pFrozen->GetColRect());

						if (isHitEnemy)
						{
							m_pFrozen = nullptr;
						

							//インスタンスを消す
							//ボスにダメージを与える
							m_pWolf->HitBossDamage(20);


						}
					}
				
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
			//動いていないときに敵と当たる//オークライダー
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
			//動いていないときに敵と当たる//どくろアーチャー
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
			//ボスたちはそれをするか迷い中
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

	for (auto& frozen : m_pFrozens)
	{
		if (!frozen)continue;

		//ボスの弾と当たったとき
		for (auto& e_shot : m_pBossShots)
		{
			if (!frozen)continue;
			if (!e_shot)continue;
			if (e_shot->GetColRect().IsCollision(frozen->GetColRect()))
			{
				e_shot = nullptr;
				
			}
		}
		//敵の矢と当たったとき
		for (auto& e_arrow : m_pEnemyArrows)
		{
			if (!frozen)continue;
			if (!e_arrow)continue;
			if (e_arrow->GetColRect().IsCollision(frozen->GetColRect()))
			{
				e_arrow = nullptr;

			}
		}
		//サーモンと当たったとき
		for (auto& it : m_pSalmons)
		{
			if (!frozen)continue;
			if (!it)continue;
			if (it->GetColRect().IsCollision(frozen->GetColRect()))
			{
				it = nullptr;

			}
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
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
		{
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(e->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//e->DropItem(e);
				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
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
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);
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
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
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
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);
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
	//ボスエリート
	{
		if (m_pElite != nullptr)
		{
			//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
			if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
			{
				bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(m_pElite->GetColRect());
				if (isHitAttack)
				{
					m_pElite->HitBossDamage(10);

				}
			}
		}
	}
	//ボス熊
	{
		if (m_pBear != nullptr)
		{
			//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
			if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
			{
				bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(m_pBear->GetColRect());
				if (isHitAttack)
				{
					m_pBear->HitBossDamage(10);

				}
			}
		}
	}
	//狼
	{
		if (m_pWolf != nullptr)
		{
			//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
			if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 2)
			{
				bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(m_pWolf->GetColRect());
				if (isHitAttack)
				{
					m_pWolf->HitBossDamage(10);

				}
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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);


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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);


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


				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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
	//ボスエリート
	{
		if (m_pElite != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColBurningRect().IsCollision(m_pElite->GetColRect());
			if (isHitAttack)
			{
				m_pElite->HitBossDamage(20);

			}
		}
	}
	//ボス熊
	{
		if (m_pBear != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColBurningRect().IsCollision(m_pBear->GetColRect());
			if (isHitAttack)
			{
				m_pBear->HitBossDamage(20);

			}
		}
	}
	
	//ボス狼
	{
		if (m_pWolf != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColBurningRect().IsCollision(m_pWolf->GetColRect());
			if (isHitAttack)
			{
				m_pWolf->HitBossDamage(20);

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

			//アイテムを落とす処理
			auto item = std::make_shared<Item>(e);//新しくアイテムを生成
			item->SetBgPointer(m_pBg);
			item->ChangePos() = e->GetPos();
			m_pItems.push_back(item);

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

			//アイテムを落とす処理
			auto item = std::make_shared<Item>(e);//新しくアイテムを生成
			item->SetBgPointer(m_pBg);
			item->ChangePos() = e->GetPos();
			m_pItems.push_back(item);

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

			//アイテムを落とす処理
			auto item = std::make_shared<Item>(e);//新しくアイテムを生成
			item->SetBgPointer(m_pBg);
			item->ChangePos() = e->GetPos();
			m_pItems.push_back(item);

			//消えるとき絶対する処理
			//対応するspawnを復活可能にする
			EnemySpawn& spawn = FindSpawnData(e->GetInitialID());
			spawn.spawned = false;
			spawn.wasKilled = true;

			//インスタンスを消す
			m_pEnemyArchers.erase(m_pEnemyArchers.begin() + i);
		}
	}
	//ボスエリート
	{
		if (m_pElite != nullptr)
		{
			//バーニングの攻撃の矩形との当たり判定チェック
			if (CheckSweepHit(p0, p1, m_pElite->GetColRect()))
			{
				m_pElite->HitBossDamage(20);
			}
		}
	}
	//ボス熊
	{
		if (m_pBear != nullptr)
		{
			//バーニングの攻撃の矩形との当たり判定チェック
			if (CheckSweepHit(p0, p1, m_pBear->GetColRect()))
			{
				m_pBear->HitBossDamage(20);
			}
		}
	}
	//ボス狼
	{
		if (m_pWolf != nullptr)
		{
			//バーニングの攻撃の矩形との当たり判定チェック
			if (CheckSweepHit(p0, p1, m_pWolf->GetColRect()))
			{
				m_pWolf->HitBossDamage(20);
			}
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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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

				//アイテムを落とす処理
				auto item = std::make_shared<Item>(e);//新しくアイテムを生成
				item->SetBgPointer(m_pBg);
				item->ChangePos() = e->GetPos();
				m_pItems.push_back(item);

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
	for (int i = (int)m_pBossShots.size() - 1; i >= 0; i--)//ボスの弾
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pBossShots[i];
			if (e == nullptr)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(e->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(e));


			

				//インスタンスを消す
				m_pBossShots.erase(m_pBossShots.begin() + i);
			}

		}
	}
	for (int i = (int)m_pSalmons.size() - 1; i >= 0; i--)//サーモン
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			auto& e = m_pSalmons[i];
			if (e == nullptr)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(e->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(e));


			

				//インスタンスを消す
				m_pSalmons.erase(m_pSalmons.begin() + i);
			}

		}
	}
	//ボスエリート
	{
		if (m_pElite != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColFrozenRect().IsCollision(m_pElite->GetColRect());
			if (isHitAttack)
			{
				m_pElite->HitBossDamage(20);

			}
		}
	}
	//ボス熊
	{
		if (m_pBear != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColFrozenRect().IsCollision(m_pBear->GetColRect());
			if (isHitAttack)
			{
				m_pBear->HitBossDamage(20);

			}
		}
	}
	//ボス狼
	{
		if (m_pWolf != nullptr)
		{
			bool isHitAttack = m_pPlayer->GetColFrozenRect().IsCollision(m_pWolf->GetColRect());
			if (isHitAttack)
			{
				m_pWolf->HitBossDamage(20);

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
	//波動
	for (auto& wm : m_waveManagers)//波動マネ
	{
		for (auto& wave : wm->GetWaves())//波動
		{
			if (wave->IsDead())continue;

			if (m_pPlayer->GetColRect().IsCollision(wave->GetColRect()))
			{

				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(wave->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
	}

	//エリートボス
	{
		//ボスの攻撃
		if (m_pElite != nullptr)
		{
			if (m_pElite->GetColAttack1Rect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pElite->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
		//ボスとの当たり判定
		if (m_pElite != nullptr && !m_pElite->GetIsDead())
		{
			if (m_pElite->GetColRect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pElite->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
	}
	//熊
	{
		//ボスの攻撃
		if (m_pBear != nullptr)
		{
			if (m_pBear->GetColAttack1Rect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pBear->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
		//ボスとの当たり判定
		if (m_pBear != nullptr&& !m_pBear->GetIsDead())
		{
			if (m_pBear->GetColRect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pBear->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
	}
	//狼
	{
		//ボスの攻撃
		if (m_pWolf != nullptr)
		{
			if (m_pWolf->GetColAttack1Rect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pWolf->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
		//ボスとの当たり判定
		if (m_pWolf != nullptr && !m_pWolf->GetIsDead())
		{
			if (m_pWolf->GetColRect().IsCollision(m_pPlayer->GetColRect()))
			{
				bool isLeft = m_pPlayer->GetColRect().CheckLeftHit(m_pWolf->GetColRect());
				//敵がどっちから当たったかどうかを入れる
				//プレイヤーのダメージ処理
				m_pPlayer->DamageHit(isLeft);
				OnShake();
			}
		}
	}
	
	
	//回復アイテムを取ったときの反応
	for (auto& potion : m_pPotions)
	{
		if (!potion)continue;

		if (m_pPlayer->GetColRect().IsCollision(potion->GetColRect()))
		{
			m_pPlayer->HealGet(100);
			potion = nullptr;
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
			stageUI.Init(0, m_pPlayer->GetType(), m_pPlayer->GetLife());//HPを0にする
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
	for (auto& item : m_pItems)
	{
		if (!item)continue;
			//プレイヤーがコピー状態かつ変身アニメーションの特定フレーム以降の当たり判定をチェック
			if (m_pPlayer->GetState() == PlayerState::Copy)
			{
				if (m_pPlayer->GetAnimIdx() > 3)
				{
					//プレイヤーの当たり判定とアイテムの当たり判定をチェック
					bool isHitItem = m_pPlayer->GetColCopyRect().IsCollision(item->GetColRect());
					if (isHitItem)
					{
						//ここにアイテムを取得したときの処理を書く
						if (item->GetItemState() == ItemState::Burning)//バーニングのアイテムでの変身
						{

							m_pPlayer->ChangeBurning();
							item = nullptr;
							//ここでアップデートを変える
							update_ = &GameScene::CopyingUpdate;
							draw_ = &GameScene::NormalDraw;
							m_frame = 0;
							return;

						}
						else if (item->GetItemState() == ItemState::Frozen)//フローズンのアイテムでの変身
						{

							m_pPlayer->ChangeFrozen();
							item = nullptr;
							//ここでアップデートを変える
							update_ = &GameScene::CopyingUpdate;
							draw_ = &GameScene::NormalDraw;
							m_frame = 0;
							return;
						}
						else if (item->GetItemState() == ItemState::Archer)//アーチャーのアイテムでの変身
						{

							m_pPlayer->ChangeArcher();
							item = nullptr;
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

	if (input.IsTriggered("Jump"))
	{
		m_pFishers.clear();
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3000,900 }, 1));
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3300,900 }, 2));
		m_pFishers.push_back(std::make_shared<Fish>(Vec2{ 3450,900 }, 3));
		//魚
		{
			for (auto& it : m_pFishers)
			{
				it->SetPlayer(m_pPlayer);
				it->SetBgPointer(m_pBg);
			}
		}
	}


	//復活、生成チェック
	CheckSpawns();



	//お試しテスト用
	if (input.IsTriggered("ok"))
	{
		
	}



	const auto& wsize = Application::GetInstance().GetWindowSize();
	

#ifdef _DEBUG
	if (input.IsTriggered("ok"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeOutDraw;
		m_frame = 0;
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
		controller_.SetPlayerInfo(m_pPlayer->GetType(),m_pPlayer->GetHp(),m_pPlayer->GetLife());
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}
	//カメラ
	UpdateCamera(camera, m_pPlayer);
	CheckBossCamera();
	//背景
	m_pBg->Update();
	//ドア
	m_doors->Update();
	//ステージUI
	{
		//プレイヤーのHPを引き渡す
		stageUI.SetHp(m_pPlayer->GetHp());
		//プレイヤーのTypeを引き渡す
		stageUI.SetType(m_pPlayer->GetType());
		///プレイヤーのlifeを引き渡す
		stageUI.SetLife(m_pPlayer->GetLife());

		//ボスのHPを引き渡す
		if (camera.isBoss)stageUI.SetBossHp(m_pElite->GetHp());

		//UIのアップデート
		stageUI.Update();
	}

	//プレイヤー関連
	{
	
		//プレイヤーのHpが0以下だったら死ぬ
		if (m_pPlayer->GetHp() <= 0)
		{
			DyingAct();
			return;
		}
		//プレイヤーが落ちたかどうか
		if (CheckDropped())return;//落ちてたらそのあとの処理をしない
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
		if (m_pElite != nullptr)//ボス
		{
			m_pElite->Update();

			if (m_pElite->GetIsThrow())
			{
				//投げるものを生成
				m_pBossShots.push_back(std::make_shared<BossShot>(m_pElite->Getm_isRight(),m_pElite->GetPos()));
			}

			if (m_pElite->GetIsDead())
			{
				m_pElite = nullptr;
				camera.ChangeIsBossFalse();
			}
		}
		if (m_pBear != nullptr)//ボス熊
		{
			m_pBear->Update();

		

			if (m_pBear->GetIsDead())
			{
				m_pBear = nullptr;
				
			}
		}
		if (m_pWolf != nullptr)//ボス狼
		{
			m_pWolf->Update();

		

			if (m_pWolf->GetIsDead())
			{
				m_pWolf = nullptr;
				
			}
		}
		//salmonを消す処理
		{
			m_pSalmons.erase(
				std::remove_if(
					m_pSalmons.begin(), m_pSalmons.end(),
					[](const std::shared_ptr<Salmon>& salmon)
					{
						return salmon == nullptr;
					}
				), m_pSalmons.end()
			);
		}
		for (auto& it : m_pSalmons)
		{
			it->Update();
		}
		//死んだら消す処理を後で入れる
		for (auto& it : m_pFishers)
		{
			it->Update();
		}

		for (auto& wm : m_waveManagers)
		{
			wm->Update(m_pBg);
		}
		//m_waveManagerの消す処理
		{
			m_waveManagers.erase(
				std::remove_if(
					m_waveManagers.begin(), m_waveManagers.end(),
					[](const std::unique_ptr<WaveManager>& wm)
					{
						return wm->IsFinished();
					}
				), m_waveManagers.end()
			);
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
			arrow->CheckEnemys(m_pElite);
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
		for (auto& shot : m_pBossShots)//ボスの弾のアップデート
		{

			if (!shot) continue;
			if (shot->isAlive == false)//矢が消えてるサインが出たらけす
			{
				shot = nullptr;
				continue;
			}
			//敵を渡すようにする
			shot->Update();
			shot->CheckPlayer(m_pPlayer);

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
	
	//エフェクト
	for (auto effect = m_pEffects.begin(); effect != m_pEffects.end(); )
	{
		(*effect)->Update();

		if ((*effect)->IsDead())
			effect = m_pEffects.erase(effect);  // ← 安全に削除
		else
			++effect;
	}
	//アイテム関連
	{
		//nullptrのアイテムをけす
		m_pItems.erase(
			std::remove_if(
				m_pItems.begin(), m_pItems.end(),
				[](const std::shared_ptr<Item>& it)
				{
					return it == nullptr;
				}),
			m_pItems.end()
		);
		for (auto item = m_pItems.begin(); item != m_pItems.end(); )
		{
		
			//nullptrのアイテムをけす、違ったらUpdate
			if (*item == nullptr)
				item = m_pItems.erase(item);  // ← 安全に削除(いらないけど一応(上でけしてるから))
			else
			{
				(*item)->Update();
				++item;
			}
		}
		if (m_pDroppedItem)//演出のアイテムのアップデート
		{
			m_pDroppedItem->DroppedUpdate();
			if (m_pDroppedItem->IsDead())
			{
				m_pDroppedItem = nullptr;
			}
		}
		//nullptrのアイテムを消す
		m_pPotions.erase(
			std::remove_if(
				m_pPotions.begin(), m_pPotions.end(),
				[](const std::shared_ptr<Potion>& it)
				{
					return it == nullptr;
				}),
			m_pPotions.end()
		);

		for (auto& potion : m_pPotions)
		{
			potion->Update();
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
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum+1, m_pPlayer->GetType(), m_pPlayer->GetHp(),m_pPlayer->GetLife()));
			return;
			break;
		case 2:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum + 1, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		case 3:
			controller_.ChangeScene(std::make_shared<GameClearScene>(controller_, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		case 6:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum + 1, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		case 7:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum + 1, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		case 8:
			controller_.ChangeScene(std::make_shared<GameClearScene>(controller_, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		case 9:
			controller_.ChangeScene(std::make_shared<StageSelectScene>(controller_, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
			return;
			break;
		default:
			assert(false && "ステージ移行に失敗");
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
		//ライフを1減らす
		m_pPlayer->LifeDeg();
		//Lifeが0以下になったら
		if (m_pPlayer->CheckLife())
		{
			delete m_pBg;
			controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
			return;
		}
		//delete m_pCharacter;
		delete m_pBg;
		switch (m_stageNum)
		{
		case 1:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 2:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 3:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_,m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 6://ステージ2_1
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 7:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 8:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
			return;
			break;
		case 9:
			controller_.ChangeScene(std::make_shared<GameScene>(controller_, m_stageNum, PlayerType::Normal, 100, m_pPlayer->GetLife()));
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
		
		for (auto& item : m_pItems)
		{
			if (!item)continue;
			item->Draw(camera);
		}
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
		for (auto& shot : m_pBossShots)//ボスの弾
		{
			if (!shot) continue;
			shot->Draw(camera);
		}
		for (auto& wm : m_waveManagers)//波動攻撃
		{
			wm->Draw(camera);
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
		for (auto& it : m_pSalmons)//鮭
		{
			it->Draw(camera);
		}
		for (auto& it : m_pFishers)
		{
			it->Draw(camera);
		}
		if (m_pElite != nullptr)//ボス
		{
			m_pElite->Draw(camera);
		}
		for (auto& potion : m_pPotions)
		{
			if (potion)
			potion->Draw(camera);
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
		for (auto& it : m_pSalmons)//鮭
		{
			if(it)it->Draw(camera);
		}
		for (auto& it : m_pFishers)
		{
			it->Draw(camera);
		}
		if (m_pElite != nullptr)//ボス
		{
			m_pElite->Draw(camera);
		}
		if (m_pBear != nullptr)//ボス熊
		{
			m_pBear->Draw(camera);
		}
		if (m_pWolf != nullptr)//ボス狼
		{
			m_pWolf->Draw(camera);
		}
		//アイテム
		for (auto& item : m_pItems)
		{
			if (!item)continue;
			item->Draw(camera);
		}
		//回復アイテム
		for (auto& potion : m_pPotions)
		{
			if (potion)
				potion->Draw(camera);
		}
		if (m_pDroppedItem) m_pDroppedItem->DroppedDraw(camera);
		//氷
		for (auto& m_pFrozen : m_pFrozens)
		{
			if (m_pFrozen) m_pFrozen->Draw(camera);
		}
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
		for (auto& shot : m_pBossShots)//ボスの弾
		{
			if (!shot) continue;
			shot->Draw(camera);
		}
		for (auto& m_pBurningObject : m_pBurningObjects)
		{
			if (m_pBurningObject) m_pBurningObject->Draw(camera);
		}
		ReactionBurning();
		for (auto& wm : m_waveManagers)//波動攻撃
		{
			wm->Draw(camera);
		}
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

void GameScene::CheckBossCamera()
{
	//ステージ8専用処理

	



	if (m_stageNum != 8)return;





	//ボスがまだ生きていたらの話
	if (m_pElite == nullptr)return;

	//プレイヤーはカメラの外にいけない
	if (m_pPlayer->GetPos().x < 0 + -camera.drawOffset.x)
	{
		m_pPlayer->ChangePos().x = -camera.drawOffset.x;
	}
	if (m_pPlayer->GetPos().x > kScreenWidth + -camera.drawOffset.x)
	{
		m_pPlayer->ChangePos().x = kScreenWidth + -camera.drawOffset.x;
	}


	SetBossCamera(camera);
	if (m_pPlayer->GetColRect().IsCollision(camera.m_cameraRect))
	{
		//カメラを固定する
		camera.ChangeIsBossTrue();

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
