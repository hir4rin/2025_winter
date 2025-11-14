#include "SceneMain.h"
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
	
	m_pPlayer = new Player;
	m_pBg = new Bg(m_pPlayer);
	m_pEnemyWizard.resize(2);
	for (auto& enemy : m_pEnemyWizard)
	{
		enemy = new EnemyWizard;
		enemy->SetBgPointer(m_pBg);
	}
	m_pEnemyWizard[1]->GetPos().x += 100;
	m_pItem = new Item;
	m_arrows.reserve(Arrow::Num);
	for (auto& arrow : m_arrows)
	{
		arrow = new Arrow;
		//敵の情報を送る
		arrow->SetEnemyWizard(m_pEnemyWizard);
		arrow->SetPlayer(m_pPlayer);
	}

	
	m_pPlayer->SetBgPointer(m_pBg);
	m_pItem->SetBgPointer(m_pBg);
}

SceneMain::~SceneMain()
{
	delete m_pPlayer;
	delete m_pItem;
	for (auto& num : m_pEnemyWizard)
	{
		delete num;
	}
	for (auto& num : m_arrows)
	{
		delete num;
	}
	delete m_pCharacter;
	delete m_pBg;
}

void SceneMain::Init()
{

}
void SceneMain::Update()
{

	m_pBg->Update();
	input.Update();
	m_pPlayer->Update(input);
	if (m_pPlayer->isArrowAttack)
	{
		m_pPlayer->ShotArrow(m_arrows);
		m_pPlayer->isArrowAttack = false;

	}
	for (auto& num : m_pEnemyWizard)
	{
		if (num) num->Update();
	}

	if (m_pItem) m_pItem->Update();
	for (auto& arrow : m_arrows)
	{
		arrow->Update();
	}
	CheckHit();
	CheckArrowHit();


}
void SceneMain::Draw()
{
	m_pBg->Draw();

	m_pPlayer->Draw();
	for (auto& num : m_pEnemyWizard)
	{
		if (num) num->Draw();
	}
	if (m_pItem) m_pItem->Draw();
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
			CheckHitNormal(m_pEnemyWizard);
			break;
		case PlayerType::Burning:
			CheckHitBurning(m_pEnemyWizard);
			break;
		case PlayerType::Frozen:
			CheckHitFrozen(m_pEnemyWizard);
			break;
	}
	if (m_pItem)
	{
		//プレイヤーがコピー状態かつ変身アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Copy)
		{
			if (m_pPlayer->GetAnimIdx() > 3)
			{
				//プレイヤーの当たり判定とアイテムの当たり判定をチェック
				bool isHitItem = m_pPlayer->GetColCopyRect().IsCollision(m_pItem->GetColRect());
				if (isHitItem)
				{
					//ここにアイテムを取得したときの処理を書く
					delete m_pItem;
					m_pItem = nullptr;
					m_pPlayer->ChangeBurning();
					//m_pPlayer->ChangeFrozen();
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

		EnemyWizard* enemy = num->hitEnemy;

		//敵リストから一致するやつを探して削除
		for (auto& e : m_pEnemyWizard)
		{
			if (e == enemy)
			{
				delete e;
				e = nullptr;
				break;
			}
		}
		//矢のヒット情報をリセット
		num->hitEnemy = nullptr;
	}
}

void SceneMain::CheckHitNormal(std::vector<EnemyWizard*>& enemyWizards)
{
	for (auto& num : m_pEnemyWizard)
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			if (num == nullptr)continue;
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(num->GetColRect());

		
			//矢の処理は別の場所(CheckhitArrow)

			if (isHitAttack)
			{
				//ここに敵が攻撃されたときの処理を書く
				delete num;
				num = nullptr;
			}
		
		}
	}
}

void SceneMain::CheckHitBurning(std::vector<EnemyWizard*>& enemyWizards)
{
	for (auto& num : m_pEnemyWizard)
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 0)
		{
			if (num == nullptr)continue;

			bool isHitBurning = m_pPlayer->GetColBurningRect().IsCollision(num->GetColRect());

			if (isHitBurning)
			{
				//ここに敵が攻撃されたときの処理を書く
				delete num;
				num = nullptr;
			}

		}
	}
}

void SceneMain::CheckHitFrozen(std::vector<EnemyWizard*>& enemyWizards)
{
	for (auto& num : m_pEnemyWizard)
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
				delete num;
				num = nullptr;
			}

		}
	}
}
