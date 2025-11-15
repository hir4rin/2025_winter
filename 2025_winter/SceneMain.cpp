#include "SceneMain.h"
#include "Camera.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "Item.h"
#include "DxLib.h"
#include "Arrow.h"
#include "Bg.h"

SceneMain::SceneMain():
	input{}
{
	
	m_pPlayer = std::make_shared<Player>();
	m_pBg = new Bg(m_pPlayer);
	m_pEnemyWizards.resize(2);
	for (auto& enemy : m_pEnemyWizards)
	{
		enemy = std::make_shared<EnemyWizard>();
		enemy->SetBgPointer(m_pBg);
	}
	m_pEnemyWizards[1]->AddPos(Vec2{ 100.0f,0.0f });
	//m_pItems = std::make_shared<Item>();
	m_arrows.reserve(Arrow::Num);
	for (auto& arrow : m_arrows)
	{
		arrow = new Arrow;
		//敵の情報を送る
		arrow->SetEnemyWizard(m_pEnemyWizards);
		arrow->SetPlayer(m_pPlayer);
	}

	
	m_pPlayer->SetBgPointer(m_pBg);
	//m_pItems->SetBgPointer(m_pBg);
}

SceneMain::~SceneMain()
{
	
	
	
	for (auto& num : m_arrows)
	{
		delete num;
	}
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
		m_pPlayer->ShotArrow(m_arrows);
		m_pPlayer->isArrowAttack = false;
	}
	for (auto& num : m_pEnemyWizards)
	{
		if (num) num->Update();
	}

	if (m_pItems) m_pItems->Update();
	for (auto& arrow : m_arrows)
	{
		arrow->Update();
	}
	CheckHit();
	CheckArrowHit();
	CheckItemWizard();
}

void SceneMain::Draw()
{
	m_pBg->Draw();

	m_pPlayer->Draw(camera);
	for (auto& num : m_pEnemyWizards)
	{
		if (num) num->Draw();
	}
	if (m_pItems) m_pItems->Draw();
	for (auto& arrow : m_arrows)
	{
		arrow->Draw();
	}

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
					m_pPlayer->ChangeFrozen();
					//m_pPlayer->ChangeArcher();

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

void SceneMain::CheckHitNormal(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (auto& num : m_pEnemyWizards)
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
	for (auto& num : m_pEnemyWizards)
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			if (num == nullptr)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(num->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
			
				num = nullptr;
			}

		}
	}
}

void SceneMain::CheckHitFrozen(std::vector<std::shared_ptr<EnemyWizard>>& enemyWizards)
{
	for (auto& num : m_pEnemyWizards)
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
				
				num = nullptr;
			}

		}
	}
}
