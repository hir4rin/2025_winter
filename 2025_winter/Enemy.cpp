#include "Enemy.h"

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置

}

Enemy::Enemy()
{
	m_pos = kInitPos;
	m_handle = LoadGraph("data/player.png");
}

Enemy::~Enemy()
{

}

void Enemy::Init()
{

}
void Enemy::Update()
{

}
void Enemy::Draw()
{

}
