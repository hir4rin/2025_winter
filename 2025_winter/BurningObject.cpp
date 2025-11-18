#include "DxLib.h"
#include "BurningObject.h"
#include "Camera.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"


namespace
{
	constexpr int enemy_cut_w = 32;
	constexpr int enemy_cut_h = 32;
	constexpr float kScale = 4.0f;
}

BurningObject::BurningObject(std::shared_ptr<EnemyWizard> _enemyWiz):
	m_burningObjectDir(false)
{
	m_handle = LoadGraph("data/BurningBox.png");
	m_pos = _enemyWiz->GetPos();
	m_vel.y = -10.0f;
}
BurningObject::BurningObject(std::shared_ptr<EnemyRider> _enemyRiders):
	m_burningObjectDir(false)
{
	m_handle = LoadGraph("data/BurningBox.png");
	m_pos = _enemyRiders->GetPos();
	m_vel.y = -10.0f;
}

BurningObject::~BurningObject()
{
}

void BurningObject::Init()
{
	
}
void BurningObject::Update()
{
	Character::SetRect();
	//やられモーションの動き
	// 重力を受ける
	m_vel.y += 0.4f;
	m_vel.x += 0.1f;

	m_pos += m_vel;

}
void BurningObject::Draw()
{

}
void BurningObject::Draw(Camera& camera)
{


	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - enemy_cut_h/3 * 2,
	0, 0,
	enemy_cut_w, enemy_cut_h,//切り取りの幅
	kScale, 0.0f,//左が拡大率、右が回転率
	m_handle,
	true,
	m_burningObjectDir ? false : true,//反転するかどうか
		false);

	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);


}
