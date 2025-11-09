#include "DxLib.h"
#include "Character.h"


namespace
{
	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ
}


Character::Character():
	m_handle(-1),
	m_isRight(true),
	m_isGround(false),
	m_isJumpPreparing(false),
	m_jumpFrame(0),
	m_x(0),
	m_y(0),
	m_pos(m_x,m_y),
	m_vel(0,0),
	zero(0,0)
{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
}

Character::~Character()
{
}

void Character::Init()
{
}

void Character::Update()
{
	//重力処理
	Gravity();
	//当たり判定更新
	SetRect();

	m_pos += m_vel;
	if (m_pos.y > kGround)
	{
		m_pos.y = kGround;
		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		
		
	}

}

void Character::Draw()
{
#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.Draw(GetColor(0, 255, 255), false);

#endif 
}

void Character::Gravity()
{
	m_vel.y += 1.5f;
}

void Character::SetRect()
{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
}
