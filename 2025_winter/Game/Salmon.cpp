#include "Salmon.h"
#include "DxLib.h"
#include "Camera.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include <cassert>


namespace
{
	constexpr int enemy_cut_w = 128;
	constexpr int enemy_cut_h = 64;
	constexpr float kScale = 0.8f;

	//画像ずらし量
	constexpr float offsetX = 12.8;
	constexpr float offsetY = 6.4;
	//描画幅  = 128 × 0.8 = 102.4
	//描画高さ = 64 × 0.8 = 51.2
	//offsetX = (128 - 102.4) / 2 = 12.8
	//offsetY = (64 - 51.2) / 2 = 6.4


	constexpr float kLowSpeed = 5.0f;
	constexpr float kMidSpeed = 8.0f;
	constexpr float kHighSpeed = 0.1f;
}

Salmon::Salmon(Vec2 pos,bool dir,int num):
	m_angle(0)
{
	m_handle = LoadGraph("data/Game/salmon.png");
	assert(m_handle >= 0);

	m_pos = pos;
	m_isGround = dir;
	
	m_Num = num;
	switch (m_Num)
	{
	case 1://低い
		m_vel.y = -8.0f;
	
		m_vel.x += m_isGround ? +kLowSpeed : -kLowSpeed;
		break;
	case 2://中
		m_vel.y = -12.0f;
		m_vel.x += m_isGround ? +kMidSpeed : -kMidSpeed;
		break;
	case 3://高い
		m_vel.y = -18.0f;
		break;
	}
}

Salmon::~Salmon()
{
	DeleteGraph(m_handle);
}

void Salmon::Init()
{
}

void Salmon::Update()
{
	//当たり判定
	m_colRect.SetCenter(m_pos.x, m_pos.y, 50.0f, 50.0f);
	//やられモーションの動き
	// 重力を受ける
	m_vel.y += 0.4f;
	switch (m_Num)
	{
	case 1://低い
	{
		if (m_isGround)
		{
			
			m_angle += -15.0f;
		}
		else
		{
		
			m_angle += +15.0f;
		}
	}
		break;
	case 2://中
	{
		if (m_isGround)
		{
	
			m_angle += -10.0f;
		}
		else
		{
			
			m_angle += +10.0f;
		}
	}
		break;
	case 3://高い
	{
		if (m_isGround)
		{
			m_vel.x += kHighSpeed;
			m_angle += -5.0f;
		}
		else
		{
			m_vel.x += -kHighSpeed;
			m_angle += +5.0f;
		}
	}
		break;

	}



	m_pos += m_vel;
}

void Salmon::Draw()
{

}
void Salmon::Draw(Camera& camera)
{
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - enemy_cut_h / 3 * 2,
	0, 0,
	enemy_cut_w, enemy_cut_h,//切り取りの幅
	kScale, m_angle * DX_PI / 180.0f,//左が拡大率、右が回転率
	m_handle,
	true,
	m_isGround ? false : true,//反転するかどうか
		false);

#ifdef _DEBUG
	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif

}
