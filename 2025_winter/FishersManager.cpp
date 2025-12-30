#include "FishersManager.h"
#include "Bg.h"
#include "Camera.h"
#include "Fish.h"



FishersManager::FishersManager(const Vec2& startPos, int dir):
	m_fishIndex(0),
	m_phase(FisherPhase::FirstFish),
	m_startPos(startPos)
{

}

FishersManager::~FishersManager()
{

}

void FishersManager::Update(Bg* bg)
{
	//魚を出すと同時にゲームループを完成
	if (!m_pfish)
	{
		SpawnFish(bg);
		return;
	}

	//
	if (m_pfish)m_pfish->Update();

	//倒されたかどうかのチェック
	if (m_pfish->GetisDead())
	{
		m_pfish = nullptr;
		m_fishIndex++;
	}




}

void FishersManager::Draw(Camera& camera)
{
	//魚を描画させる
}



void FishersManager::SpawnFish(Bg* bg)
{
	
}
