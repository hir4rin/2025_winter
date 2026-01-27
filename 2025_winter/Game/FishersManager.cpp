#include "FishersManager.h"
#include "Bg.h"
#include "Camera.h"
#include "Fish.h"
#include "Application.h"



FishersManager::FishersManager(const Vec2& startPos, int dir):
	m_fishIndex(0),
	m_phase(FisherPhase::FirstFish),
	m_startPos(startPos),
	m_isSpawn(false),
	m_triSpawn(false),
	m_isfusion(false),
	m_publicHp(100)
{
	
}

FishersManager::~FishersManager()
{

}

void FishersManager::Update(Bg* bg)
{
	//魚を出すと同時にゲームループを完成
	if (!m_isSpawn)
	{
		SpawnFish(bg);
		m_isSpawn = true;
		//エフェクトを出す
		for (auto& func : onLandEvents)
		{
			if (func)func();//呼び出し
		}
	}

	//
	for (auto& it : m_pFishers)
	{
		if (!it)continue;
		it->Update();
	}

	if (m_phase == FisherPhase::Fusion)
	{
		
		bool allDead = true;
		for (auto& it : m_pFishers)
		{
			if (!it)continue;
			if (!it->GetisDead()) {
				allDead = false;
				break;
			}
		}
		if (allDead)
		{
			for (auto& it : m_pFishers)
			{
				if (!it)continue;
				it = nullptr;
			}

			//すべて、消えた
			m_isSpawn = false;
			m_phase = FisherPhase::SpecialFish;
			//Bgm
			Application::GetInstance().GetSoundManager().PlayBgm("bgmFishAfter");
		}


	}



	if (m_phase == FisherPhase::SpecialFish && m_fishIndex == 1)
	{
		bool allDead = true;
		for (auto& it : m_pFishers)
		{
			if (!it)continue;
			if (!it->GetisDead()) {
				allDead = false;
				break;
			}
		}
		if (allDead)
		{
			for (auto& it : m_pFishers)
			{
				if (!it)continue;
				it = nullptr;
			}
			
			//すべて、消えた
			m_isSpawn = false;
			m_phase = FisherPhase::FirstFish;
			m_fishIndex++;//2になる
		}



	}

	

	//倒されたかどうかのチェック
	for (auto& it : m_pFishers)
	{

		if (!it)continue;
		if (it->GetisDead())
		{
			m_publicHp += -12;
			if (m_publicHp <= 4)m_publicHp = 0;
		

			switch (m_phase)
			{
			case FisherPhase::FirstFish:
				m_phase = FisherPhase::SecondFish;
				it = nullptr;
				m_isSpawn = false;
				break;
			case FisherPhase::SecondFish:
				m_phase = FisherPhase::ThirdFish;
				it = nullptr;
				m_isSpawn = false;
				break;
			case FisherPhase::ThirdFish:
				m_phase = FisherPhase::Fusion;
				it = nullptr;
				m_isSpawn = false;
				break;
			case FisherPhase::Fusion:
			
				break;
			case FisherPhase::SpecialFish:
				if (m_fishIndex == 0)
				{
					m_triSpawn = true;
					ReleasePos = it->GetPos();
				
				}
				else
				{
					m_triSpawn = true;
				
					ReleasePos = it->GetPos();
				}
				
				it = nullptr;
				break;
			case FisherPhase::End:
				
				break;
			}
		}

	
	}

	if (m_triSpawn)
	{
		if (m_fishIndex == 0)
		{
			//三体の魚は着陸
			m_pFishers.push_back(std::make_shared<Fish >(Vec2 {ReleasePos.x - 100,ReleasePos.y}, 1, 50, FishState::Release));
			m_pFishers.push_back(std::make_shared<Fish >(Vec2{ ReleasePos.x,ReleasePos.y }, 2, 50, FishState::Release));
			m_pFishers.push_back(std::make_shared<Fish >(Vec2{ ReleasePos.x + 100,ReleasePos.y }, 3, 50, FishState::Release));
			//SE
			Application::GetInstance().GetSoundManager().PlaySE("droppedSE");

			m_fishIndex++;
			m_triSpawn = false;
			//BGm変える
			//Bgm
			Application::GetInstance().GetSoundManager().PlayBgm("bgmFishBefore");
		}
		else
		{
			//三体の魚を爆散させる
			m_pFishers.push_back(std::make_shared<Fish >(Vec2{ ReleasePos.x - 100,ReleasePos.y }, 1, 50, FishState::Dead));
			m_pFishers.push_back(std::make_shared<Fish >(Vec2{ ReleasePos.x,ReleasePos.y }, 2, 50, FishState::Dead));
			m_pFishers.push_back(std::make_shared<Fish >(Vec2{ ReleasePos.x + 100,ReleasePos.y }, 3, 50, FishState::Dead));
			//2回で終わり
			m_triSpawn = false;
			m_fishIndex++;
			m_phase = FisherPhase::End;
			m_isDead = true;
			//Bgm
			//Application::GetInstance().GetSoundManager().PlayBgm("bgm");
			
		}
	
	}


	
	


}

void FishersManager::Draw(Camera& camera)
{
	for (auto& it : m_pFishers)
	{
		if (!it)continue;
		it->Draw(camera);
	}
}



void FishersManager::SpawnFish(Bg* bg)
{
	switch (m_phase)
	{
	case FisherPhase::FirstFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 1540,900 }, 1));
		break;
	case FisherPhase::SecondFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 1540,900 }, 2));

		break;
	case FisherPhase::ThirdFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 1540,900 }, 3));

		break;
	case FisherPhase::Fusion:
		//ここで三人出す
		m_pFishers.push_back(std::make_shared<Fish >(Vec2{ 1440,900 }, 1,50,FishState::Fusion));
		m_pFishers.push_back(std::make_shared<Fish >(Vec2{ 1540,900 }, 2,50,FishState::Fusion));
		m_pFishers.push_back(std::make_shared<Fish >(Vec2{ 1640,900 }, 3,50,FishState::Fusion));
		break;
	case FisherPhase::SpecialFish:
	

		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 1540,900 }, 4));
		//ここでカウントして、何回目かで終わらせる
		break;
	case FisherPhase::End:
		break;
	}
	//SetBgやSetPlayerはUpdate	でやる
	
	
}
