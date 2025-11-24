#include "GameScene.h"
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
#include "Bg.h"
#include "../input.h"
#include "GameoverScene.h"
#include "SceneController.h"
#include <cassert>
#include "../Application.h"

namespace
{
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;

	constexpr int fade_interval = 60;


	constexpr float FrozenSpeed = 13.0f;
}


GameScene::GameScene(SceneController& controller) :
	Scene(controller),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw)

{
	//実質Initの使い方
	m_enemySpawns.push_back({ EnemyType::Wizard, Vec2(1500.0f,500.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard, Vec2(2100.0f,700.0f), false });
	m_enemySpawns.push_back({ EnemyType::Archer, Vec2(2500.0f,700.0f), false });
	m_enemySpawns.push_back({ EnemyType::Wizard, Vec2(3000.0f,500.0f), false });
	m_enemySpawns.push_back({ EnemyType::Rider, Vec2(4000.0f,700.0f), false });

	InitCamera(camera);//カメラの初期化
	//-----------------------------------------------------------------
	frame_ = fade_interval;// フェードインの最初
	m_pPlayer = std::make_shared<Player>();
	m_pBg = new Bg(m_pPlayer);
	//m_pEnemyWizards.resize(3);//ペンギンの数
	//for (int i = 0; i < m_pEnemyWizards.size(); i++)
	//{
	//	float  distance = i * 200;
	//	m_pEnemyWizards[i] = std::make_shared<EnemyWizard>();
	//	m_pEnemyWizards[i]->SetBgPointer(m_pBg);
	//	m_pEnemyWizards[i]->SetPlayer(m_pPlayer);
	//	m_pEnemyWizards[i]->AddPos(Vec2{ distance,0.0f });

	//}
	//m_pEnemyRiders.resize(2);//ライダーの数
	//for (int i = 0; i < m_pEnemyRiders.size(); i++)
	//{
	//	float  distance = i * 200;
	//	m_pEnemyRiders[i] = std::make_shared<EnemyRider>();
	//	m_pEnemyRiders[i]->SetBgPointer(m_pBg);
	//	m_pEnemyRiders[i]->SetPlayer(m_pPlayer);
	//	m_pEnemyRiders[i]->AddPos(Vec2{ distance,0.0f });

	//}

	/*m_pEnemyArchers.resize(3);
	for (int i = 0; i < m_pEnemyArchers.size(); i++)
	{
		float  distance = i * 200;
		m_pEnemyArchers[i] = std::make_shared<EnemyArcher>();
		m_pEnemyArchers[i]->SetBgPointer(m_pBg);
		m_pEnemyArchers[i]->SetPlayer(m_pPlayer);
		m_pEnemyArchers[i]->AddPos(Vec2{ distance,0.0f });
		i * 200;
	}*/

	//m_pItems = std::make_shared<Item>();


	//後で張り付ける
		////敵の情報を送る
		//arrow->SetEnemyWizard(m_pEnemyWizards);
		//arrow->SetPlayer(m_pPlayer);



	m_pPlayer->SetBgPointer(m_pBg);
	//m_pItems->SetBgPointer(m_pBg);
}




void GameScene::CheckHit()
{
	switch (m_pPlayer->GetType())
	{
	case PlayerType::Normal:
		CheckHitNormal(m_pEnemyWizards);
		break;
	case PlayerType::Burning:
		CheckHitBurning(m_pEnemyWizards);
		break;
	case PlayerType::Frozen:
		CheckHitFrozen(m_pEnemyWizards);
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



void GameScene::CheckArrowHit()
{
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemyWizard)continue;

		std::shared_ptr<EnemyWizard> enemy = num->hitEnemyWizard;

		//敵リストから一致するやつを探して削除
		for (auto& e : m_pEnemyWizards)
		{
			if (e == enemy)
			{

				e = nullptr;
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
		for (auto& e : m_pEnemyRiders)
		{
			if (e == enemy)
			{

				e = nullptr;
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
		for (auto& e : m_pEnemyArchers)
		{
			if (e == enemy)
			{

				e = nullptr;
				break;
			}
		}
		//矢のヒット情報をリセット
		num->hitEnemyArcher = nullptr;
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
			for (auto& enemyRider : m_pEnemyRiders)
			{
				if (!enemyRider) continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = enemyRider->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					enemyRider = nullptr;
					m_pFrozen = nullptr;

				}

			}
			//動いているときに敵と当たる//どくろアーチャー
			for (auto& enemyArcher : m_pEnemyArchers)
			{
				if (!enemyArcher) continue;
				if (!m_pFrozen) break;
				bool isHitEnemy = enemyArcher->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					enemyArcher = nullptr;
					m_pFrozen = nullptr;

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

void GameScene::CheckHitNormal(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
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
	for (auto& num : m_pEnemyRiders)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(num->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//num->DropItem(num);
				//ここに敵が攻撃されたときの処理を書く
				//if (num == nullptr || !rider->HitRider)continue;

				//アイテムを落とす処理
				m_pItems = std::make_shared<Item>(num);//新しくアイテムを生成//別のアイテムを渡す
				m_pItems->SetBgPointer(m_pBg);
				m_pItems->ChangePos() = num->GetPos();
				//敵のヒット情報をリセット
				//num->HitRider = nullptr;

				num = nullptr;

			}

		}
	}
	for (auto& num : m_pEnemyArchers)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(num->GetColRect());


			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//num->DropItem(num);
				//ここに敵が攻撃されたときの処理を書く
				//if (num == nullptr || !rider->HitRider)continue;

				//アイテムを落とす処理
				m_pItems = std::make_shared<Item>(num);//新しくアイテムを生成//別のアイテムを渡す
				m_pItems->SetBgPointer(m_pBg);
				m_pItems->ChangePos() = num->GetPos();
				//敵のヒット情報をリセット
				//num->HitRider = nullptr;

				num = nullptr;

			}

		}
	}
}

void GameScene::CheckHitBurning(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)//ペンギン
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() >= 0)
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
	for (auto& num : m_pEnemyRiders)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			if (num == nullptr)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(num->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pBurningObjects.push_back(std::make_shared<BurningObject>(num));
				num = nullptr;
			}

		}
	}
	for (auto& num : m_pEnemyArchers)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			if (num == nullptr)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(num->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pBurningObjects.push_back(std::make_shared<BurningObject>(num));
				num = nullptr;
			}

		}
	}
}

void GameScene::CheckHitFrozen(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
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
	for (auto& num : m_pEnemyRiders)//オークライダー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(num->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(num));
				num = nullptr;
			}

		}
	}
	for (auto& num : m_pEnemyArchers)//どくろアーチャー
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;

			bool isHitFrozen = m_pPlayer->GetColFrozenRect().IsCollision(num->GetColRect());
			//矢の処理は別の場所(CheckhitArrow)


			if (isHitFrozen)
			{
				//ここに敵が攻撃されたときの処理を書く
				m_pFrozens.push_back(std::make_shared<Frozen>(num));
				num = nullptr;
			}

		}
	}
}

void GameScene::FadeInUpdate(Input&)
{



	if (frame_-- <= 0)
	{

		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::NormalUpdate(Input& input)
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
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyWizards.push_back(enemy);
				}
				else if (spawn.type == EnemyType::Rider)
				{
					auto enemy = std::make_shared<EnemyRider>();
					enemy->SetBgPointer(m_pBg);
					enemy->SetPlayer(m_pPlayer);
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyRiders.push_back(enemy);
				}
				else if (spawn.type == EnemyType::Archer)
				{
					auto enemy = std::make_shared<EnemyArcher>();
					enemy->SetBgPointer(m_pBg);
					enemy->SetPlayer(m_pPlayer);
					enemy->AddPos(spawn.pos);
					enemy->SetInitialID(spawn.pos);
					m_pEnemyArchers.push_back(enemy);
				}
			}

		}
	}





	const auto& wsize = Application::GetInstance().GetWindowSize();

	if (input.IsTriggered("ok"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
	}

	//ポーズ画面
	/*if (input.IsTriggered("pause"))
	{
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}*/
	UpdateCamera(camera, m_pPlayer);


	m_pBg->Update();

	m_pPlayer->Update(input);
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
	CheckHit();//当たり判定
	CheckArrowHit();
	//CheckItemWizard();
	//CheckItemOrcRider();
	CheckFrozenHit();

	//消える処理
	for (int i = (int)m_pEnemyWizards.size() - 1; i >= 0; i--)
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





}

void GameScene::FadeOutUpdate(Input&)
{
	if (frame_++ >= fade_interval)
	{
		//delete m_pCharacter;
		delete m_pBg;
		controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
		return;
	}
}

void GameScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	m_pBg->Draw(camera);

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

	float left = camera.pos.x + 10 - screenWidth / 2;
	float right = camera.pos.x + screenWidth / 2 - 10;
	float top = camera.pos.y + 10;
	float bottom = camera.pos.y + screenHeight - 10;
	DrawBox(left + camera.drawOffset.x, top, right + camera.drawOffset.x, bottom, GetColor(255, 255, 0), false);

}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
