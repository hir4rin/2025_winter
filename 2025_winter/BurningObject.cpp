#include "DxLib.h"
#include "BurningObject.h"
#include "Camera.h"
#include "EnemyWizard.h"


namespace
{
	constexpr int enemy_cut_w = 32;
	constexpr int enemy_cut_h = 32;
	constexpr float kScale = 1.0f;
}

BurningObject::BurningObject(std::shared_ptr<EnemyWizard> _enemyWiz):
	m_burningObjectDir(false)
{
	m_handle = LoadGraph("data/BurningBox.png");
	m_pos = _enemyWiz->GetPos();
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

}
void BurningObject::Draw()
{

}
void BurningObject::Draw(Camera& camera)
{


	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	0, 0,
	enemy_cut_w, enemy_cut_h,//切り取りの幅
	kScale, 0.0f,//左が拡大率、右が回転率
	m_handle,
	true, false,
	m_burningObjectDir ? false : true);//反転するかどうか

	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);


}
