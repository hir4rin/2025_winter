#include "Frozen.h"
#include "DxLib.h"
#include "Camera.h"
#include "EnemyWizard.h"


namespace
{

	constexpr int enemy_cut_w = 396;
	constexpr int enemy_cut_h = 425;
	constexpr float kScale = 1*0.5f*0.5f*0.5f;
}

Frozen::Frozen(std::shared_ptr<EnemyWizard> _enemyWiz)
{
	m_handle = LoadGraph("data/icebox.png");
	m_pos = _enemyWiz->GetPos();
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
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		0, 0,
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle,
		true, false);

	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);

}
