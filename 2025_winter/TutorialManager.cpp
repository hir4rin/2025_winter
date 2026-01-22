#include "TutorialManager.h"
#include "Player.h"
#include "DxLib.h"
#include "Bg.h"
#include "Camera.h"
#include "ButtonUI.h"
#include "Sign.h"
#include "../EffekseerResourceManager.h"


TutorialManager::TutorialManager(Vec2 pos, TutorialPhase phase,std::shared_ptr<EffekseerResourceManager> effRes):
	m_frame(0)
{
	m_offset = Vec2{ 20,-10 };
	m_phase = phase;
	m_effRes  = effRes;
	m_startPos = pos;
	switch (m_phase)
	{
	case TutorialPhase::Move :
		m_pSign = std::make_shared<Sign>(pos, 0, 1);
		//十字キー
		m_pButton = std::make_shared<ButtonUI>("move",pos);
		break;
	case TutorialPhase::Up :
		m_pSign = std::make_shared<Sign>(pos, 0, 1);
		//上キー
		m_pButton = std::make_shared<ButtonUI>("up",pos);
		break;
	case TutorialPhase::Attack:
		m_pSign = std::make_shared<Sign>(pos, 0, 1);
		//Xボタン
		m_pButton = std::make_shared<ButtonUI>("X", pos);
		//プレイヤーを出す
		m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, pos+ m_offset, 1, effRes);
			break;
	case TutorialPhase::Copy : 
		m_pSign = std::make_shared<Sign>(pos, 0, 1);
		//Xボタン
		m_pButton = std::make_shared<ButtonUI>("Y", pos);
		//プレイヤーを出す
		m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, pos + m_offset, 1, effRes);
		break;
	case TutorialPhase::CopyOut : 
		m_pSign = std::make_shared<Sign>(pos, 0, 1);
		//Xボタン
		m_pButton = std::make_shared<ButtonUI>("Y", pos);
		//プレイヤーを出す
		m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, pos + m_offset, 1, effRes);
		m_pPlayer->ChangeFrozen();
		break;
	default:
		break;
		

	}
	//m_pSign = std::make_shared<Sign>(pos,0,1);
	//m_pPlayer = std::make_shared<Player>(PlayerType::Normal,pos,);
}

TutorialManager::~TutorialManager()
{
}
void TutorialManager::Update()
{
	m_pSign->Update();
	m_pButton->Update();
	if (m_pPlayer)
		switch (m_phase)
		{
		case TutorialPhase::Attack://攻撃
			m_pPlayer->TutorialAttackUpdate();
			break;
		case TutorialPhase::Copy://コピー
			m_pPlayer->TutorialCopyUpdate();
			if (m_pPlayer->GetTutorialFinish())
			{
				m_pPlayer = nullptr;
				//プレイヤーを作り直す
				m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, m_startPos + m_offset, 1, m_effRes);
			}
			break;
		case TutorialPhase::CopyOut://コピーアウト
			m_pPlayer->TutorialCopyOutUpdate();
			if (m_pPlayer->GetTutorialFinish())
			{
				m_pPlayer = nullptr;
				//プレイヤーを作り直す
				m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, m_startPos + m_offset, 1, m_effRes);
				m_pPlayer->ChangeFrozen();
			}
			break;
		}
}
void TutorialManager::Draw(Camera& camera)
{
	m_pSign->Draw(camera);
	m_pButton->Draw(camera);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	if (m_pPlayer)m_pPlayer->Draw(camera);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
