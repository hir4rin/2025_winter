#include "Frozen.h"
#include "DxLib.h"
#include "Camera.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "BossShot.h"
#include "Salmon.h"
#include <cassert>

namespace
{

	constexpr int enemy_cut_w = 50;
	constexpr int enemy_cut_h = 50;


	//表示させる画像
	constexpr int kCharaIdx = 15;
	
}

Frozen::Frozen(std::shared_ptr<EnemyWizard> _enemyWiz)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);

	m_pos = _enemyWiz->GetPos();
}
Frozen::Frozen(std::shared_ptr<EnemyRider> _enemyRiders)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);

	m_pos = _enemyRiders->GetPos();


}
Frozen::Frozen(std::shared_ptr<EnemyArcher> _enemyArchers)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);

	m_pos = _enemyArchers->GetPos();
}

Frozen::Frozen(std::shared_ptr<BossShot> _bossShot)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);

	m_pos = _bossShot->GetPos();
	kScale = 4.0f;
	drawY = 1.5f;
}
Frozen::Frozen(std::shared_ptr<Salmon> _salmon)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);

	m_pos = _salmon->GetPos();
	kScale = 4.0f;
	drawY = 1.5f;
}

Frozen::~Frozen()
{
}

void Frozen::Init()
{

}
void Frozen::Update()
{
	Character::SetRect();
	m_pos += m_vel;
}
void Frozen::Draw()
{
}
void Frozen::Draw(Camera& camera)
{
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y- enemy_cut_h*drawY,
		enemy_cut_w* kCharaIdx, 0,
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle,
		true, false);

#ifdef _DEBUG
	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif

}
