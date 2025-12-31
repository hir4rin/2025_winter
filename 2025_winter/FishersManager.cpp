#include "FishersManager.h"
#include "Bg.h"
#include "Camera.h"
#include "Fish.h"



FishersManager::FishersManager(const Vec2& startPos, int dir):
	m_fishIndex(0),
	m_phase(FisherPhase::FirstFish),
	m_startPos(startPos),
	m_isSpawn(false)
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
	}

	//
	for (auto& it : m_pFishers)
	{
		if (!it)continue;
		it->Update();
	}
	

	//倒されたかどうかのチェック
	for (auto& it : m_pFishers)
	{
		if (!it)continue;
		if (it->GetisDead())
		{
			it = nullptr;
			m_fishIndex++;
			m_isSpawn = false;
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
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 3000,900 }, 1));
		m_phase = FisherPhase::SecondFish;
		break;
	case FisherPhase::SecondFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 3300,900 }, 2));
		m_phase = FisherPhase::ThirdFish;

		break;
	case FisherPhase::ThirdFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 3300,900 }, 3));
		m_phase = FisherPhase::SpecialFish;

		break;
	case FisherPhase::SpecialFish:
		m_pFishers.push_back(std::make_shared<Fish > (Vec2{ 3300,900 }, 3));
		//ここでカウントして、何回目かで終わらせる

		m_phase = FisherPhase::FirstFish;

		break;
	}
	//SetBgやSetPlayerはUpdate	でやる
	
	
}
