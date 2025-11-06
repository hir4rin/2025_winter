#include "SceneMain.h"
#include "Character.h"
#include "Player.h"
#include "EnemyWizard.h"
#include "DxLib.h"
SceneMain::SceneMain()
{
	m_pPlayer = new Player;
	m_pEnemyWizard = new EnemyWizard;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{

}
void SceneMain::Update()
{
	m_pPlayer->Update();
	if (m_pEnemyWizard) m_pEnemyWizard->Update();
	CheckHit();
}
void SceneMain::Draw()
{
	m_pPlayer->Draw();
	if (m_pEnemyWizard) m_pEnemyWizard->Draw();

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
	
}
