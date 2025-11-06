#include "EnemyWizard.h"
#include "DxLib.h"

namespace
{
	const Vec2 kInitPos = { 1000.0f,100.0f };//初期位置
	constexpr float kSpeed = 20.0f;//移動速度
	constexpr int enemy_cut_w = 64;
	constexpr int enemy_cut_h = 64;
	constexpr float  enemy_scale = 2.0f;
}


EnemyWizard::EnemyWizard():
	charaIdx(0),
	charaIdy(0),
	m_animframe(0)
{
	m_handle = LoadGraph("data/enemyWizard.png");
	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Normal;
}

EnemyWizard::~EnemyWizard()
{
}

void EnemyWizard::Init()
{
}

void EnemyWizard::Update()
{
	m_animframe++;
	Character::Update();

}

void EnemyWizard::Draw()
{


	//当たり判定の描画
	Character::Draw();


	switch (_state)
	{
		case EnemyState::Normal:
		charaIdx = (m_animframe / 10) % 11;
		charaIdy = 0;
		break;
		case EnemyState::Attack:
			break;
		case EnemyState::Damage:
			break;
		default:
			break;
	}
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true, true);
	}
}
