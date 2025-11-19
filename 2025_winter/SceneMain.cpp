#include "SceneMain.h"
#include "Camera.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "Item.h"
#include "DxLib.h"
#include "Arrow.h"
#include "Frozen.h"
#include "BurningObject.h"
#include "Bg.h"


namespace
{
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;
}

SceneMain::SceneMain():
	input{}
{
	
	m_pPlayer = std::make_shared<Player>();
	m_pBg = new Bg(m_pPlayer);
	m_pEnemyWizards.resize(3);//ペンギンの数
	for (auto& enemy : m_pEnemyWizards)
	{
		static float x = 0.0f;
		enemy = std::make_shared<EnemyWizard>();
		enemy->SetBgPointer(m_pBg);
		enemy->SetPlayer(m_pPlayer);
		enemy->AddPos(Vec2{ x,0.0f });
		x += 200;
	}
	m_pEnemyRiders.resize(2);//ライダーの数
	for (auto& enemy : m_pEnemyRiders)
	{
		static float x = 0.0f;
		enemy = std::make_shared<EnemyRider>();
		enemy->SetBgPointer(m_pBg);
		enemy->SetPlayer(m_pPlayer);
		enemy->AddPos(Vec2{ x,0.0f });
		x += 200;
	}

	//m_pItems = std::make_shared<Item>();
	
	
	//後で張り付ける
		////敵の情報を送る
		//arrow->SetEnemyWizard(m_pEnemyWizards);
		//arrow->SetPlayer(m_pPlayer);
	
	
	
	m_pPlayer->SetBgPointer(m_pBg);
	//m_pItems->SetBgPointer(m_pBg);
}

SceneMain::~SceneMain()
{
	
	
	delete m_pCharacter;
	delete m_pBg;
}

void SceneMain::Init()
{
	
	InitCamera(camera);
	
}
void SceneMain::Update()
{

	UpdateCamera(camera, m_pPlayer);


	m_pBg->Update();
	input.Update();
	m_pPlayer->Update(input);
	if (m_pPlayer->isArrowAttack)
	{
		//ポインタを作ってその座標を入れる
		std::shared_ptr<Arrow> arrow = m_pPlayer->ShotArrow();
	
		//それをpush_backする
		m_arrows.push_back(arrow);

		m_pPlayer->isArrowAttack = false;
	}
	for (auto& enemy : m_pEnemyWizards)
	{
		if (enemy) enemy->Update();
	}

	for (auto& enemy : m_pEnemyRiders)
	{
		if (enemy) enemy->Update();
	}

	if (m_pItems) m_pItems->Update();
	for (auto& arrow : m_arrows)
	{
		if (!arrow) continue;
		arrow->Update();
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
	CheckItemWizard();
	CheckItemOrcRider();
	CheckFrozenHit();
}

void SceneMain::Draw()
{
	m_pBg->Draw(camera);

	for (auto& m_pFrozen : m_pFrozens)
	{
		if (m_pFrozen) m_pFrozen->Draw(camera);
	}
	m_pPlayer->Draw(camera);
	for (auto& enemy : m_pEnemyWizards)
	{
		if (enemy) enemy->Draw(camera);
	}
	for (auto& enemy : m_pEnemyRiders)
	{
		if (enemy) enemy->Draw(camera);
	}
	if (m_pItems) m_pItems->Draw(camera);
	for (auto& arrow : m_arrows)
	{
		//if (arrow) continue;
		arrow->Draw(camera);
	}
	for (auto& m_pBurningObject : m_pBurningObjects)
	{
		if (m_pBurningObject) m_pBurningObject->Draw(camera);
	}
	ReactionBurning();
}



void SceneMain::CheckHit()
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
				
					m_pItems = nullptr;
					//m_pPlayer->ChangeBurning();
					//m_pPlayer->ChangeFrozen();
					m_pPlayer->ChangeArcher();

				}
			}

		}

	}

}



void SceneMain::CheckArrowHit()
{
	for (auto& num : m_arrows)
	{
		if (num == nullptr || !num->hitEnemy)continue;

		std::shared_ptr<EnemyWizard> enemy = num->hitEnemy;

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
		num->hitEnemy = nullptr;
	}
}

void SceneMain::CheckFrozenHit()
{
	for (auto& m_pFrozen : m_pFrozens)
	{
		if (!m_pFrozen)continue;
		//プレイヤーと当たった時の反応
		bool isHitFrozen = m_pPlayer->GetColRect().IsCollision(m_pFrozen->GetColRect());
		if (isHitFrozen)
		{
			bool dir = m_pPlayer->GetPos().x < m_pFrozen->GetPos().x;
			Vec2 add = { dir ? 5.0f : -5.0f,   0 };
			m_pFrozen->AddVel(add);

			m_pFrozen->isMove = true;
		}
		if (m_pFrozen->isMove)
		{
			//動いているときに敵と当たる
			for (auto& enemyWizard : m_pEnemyWizards)
			{
				if (!enemyWizard) continue;
				bool isHitEnemy = enemyWizard->GetColRect().IsCollision(m_pFrozen->GetColRect());

				if (isHitEnemy)
				{
					enemyWizard = nullptr;
				}

			}
		}
		//ついでにカメラの外に出たら消す処理
		
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

void SceneMain::ReactionBurning()
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

void SceneMain::CheckItemWizard()
{
	for (auto& wizard : m_pEnemyWizards)
	{
		if ( wizard == nullptr || !wizard->HitWizard)continue;
		
			//アイテムを落とす処理
			m_pItems = std::make_shared<Item>();//新しくアイテムを生成
			m_pItems->SetBgPointer(m_pBg);
			m_pItems->ChangePos() = wizard->GetPos();
			//敵のヒット情報をリセット
			wizard->HitWizard = nullptr;


			
			wizard = nullptr;
	}
}

void SceneMain::CheckItemOrcRider()
{
	for (auto& rider : m_pEnemyRiders)
	{
		if (rider == nullptr || !rider->HitRider)continue;

		//アイテムを落とす処理
		m_pItems = std::make_shared<Item>();//新しくアイテムを生成//別のアイテムを渡す
		m_pItems->SetBgPointer(m_pBg);
		m_pItems->ChangePos() = rider->GetPos();
		//敵のヒット情報をリセット
		rider->HitRider = nullptr;



		rider = nullptr;
	}
}

void SceneMain::CheckHitNormal(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (auto& num : m_pEnemyWizards)//ペンギン
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(num->GetColRect());

		
			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				num->DropItem(num);
				//ここに敵が攻撃されたときの処理を書く
				
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
				num->DropItem(num);
				//ここに敵が攻撃されたときの処理を書く
				
			}
		
		}
	}
}

void SceneMain::CheckHitBurning(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (auto& num : m_pEnemyWizards)//ペンギン
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
}

void SceneMain::CheckHitFrozen(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (auto& num : m_pEnemyWizards)//ペンギン
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
}
