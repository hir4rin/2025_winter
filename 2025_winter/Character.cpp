#include "DxLib.h"
#include "Character.h"


namespace
{
	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置
	
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
	m_vel(0,0)
{
}

Character::~Character()
{
}

void Character::Init()
{
}

void Character::Update()
{
	Gravity();
	m_pos += m_vel;
	if (m_pos.y > kGround)
	{
		m_pos.y = kGround;
		m_isGround = true;
	}

}

void Character::Draw()
{
	//DrawGraphF(m_pos.x, m_pos.y, m_handle, true);
}

void Character::Gravity()
{
	m_vel.y += 1.5f;
}
