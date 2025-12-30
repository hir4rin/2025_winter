#include "WaveManager.h"
#include "Bg.h"
#include "Camera.h"
#include "Wave.h"
#include "FishersManager.h"

WaveManager::WaveManager(const Vec2& startPos, int dir):
	m_startPos(startPos),m_dir(dir)
{
}

WaveManager::~WaveManager()
{
}



void WaveManager::Update(Bg* bg)
{
	if (m_finished)return;
	m_spawnTimer++;

	if (m_spawnTimer >= m_spawnInterval)
	{
		m_spawnTimer = 0;
		SpawnWave(bg);
	}

	for (auto& w : m_waves)
	{
		w->Update();
	}
	m_waves.erase(
		std::remove_if(
			m_waves.begin(), m_waves.end(),
			[](const std::unique_ptr<Wave>& w)
			{
				return w->IsDead();
			}),
			m_waves.end()
	);


}

void WaveManager::Draw(Camera& camera)
{
	for (auto& w : m_waves)
		w->Draw(camera);
}

void WaveManager::SpawnWave(Bg* bg)
{
	if (m_step >= m_maxStep)
	{
		m_finished = true;
		return;
	}

	Vec2 pos = m_startPos;
	pos.x += m_dir * m_step * 64;//1マス=64fpx想定

	//ここで床チェック、壁チェックを後から追加
	//上下差は気にしないため、Y座標を調整
	Rect waveRect;
	waveRect.m_left = pos.x - 32;//64/2
	waveRect.m_top = pos.y-32;//64/2
	waveRect.m_right = pos.x + 32;//64/2
	waveRect.m_bottom = pos.y -32;//64/2

	Rect hitChip;
	if (bg->IsCollision(waveRect, hitChip))
	{
		//横の壁に当たった
		m_finished = true;
		return;
	}

	m_waves.push_back(std::make_unique<Wave>(pos));
	m_step++;
}

