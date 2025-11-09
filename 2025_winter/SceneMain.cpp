#include "SceneMain.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "Item.h"
#include "DxLib.h"

SceneMain::SceneMain()
{
	m_pPlayer = new Player;
	m_pEnemyWizard = new EnemyWizard;
	m_pItem = new Item;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{

}
void SceneMain::Update()
{
	input.Update();
	m_pPlayer->Update(input);
	if (m_pEnemyWizard) m_pEnemyWizard->Update();
	if (m_pItem) m_pItem->Update();
	CheckHit();
}
void SceneMain::Draw()
{
	m_pPlayer->Draw();
	if (m_pEnemyWizard) m_pEnemyWizard->Draw();
	if (m_pItem) m_pItem->Draw();

}

void SceneMain::CheckHit()
{
	if (m_pEnemyWizard)
	{
		//プレイヤーが攻撃状態かつ攻撃アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Attack && m_pPlayer->GetAnimIdx() > 3)
		{
			bool isHitAttack = m_pPlayer->GetColAttackRect().IsCollision(m_pEnemyWizard->GetColRect());

			if (isHitAttack)
			{
				//ここに敵が攻撃されたときの処理を書く
				delete m_pEnemyWizard;
				m_pEnemyWizard = nullptr;
			}
		}
	}
	if (m_pItem)
	{
		//プレイヤーがコピー状態かつ変身アニメーションの特定フレーム以降の当たり判定をチェック
		if (m_pPlayer->GetState() == PlayerState::Copy )
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
					m_pPlayer->ChangeArcher();
				}
			}
			
		}

	}

}
