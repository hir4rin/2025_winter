#include "Enemy.h"
#include "DxLib.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include <memory>
namespace
{
	const Vec2 kInitPos = { 000.0f,100.0f };//初期位置

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置


	constexpr int damageMaxFrame = 30;


}

Enemy::Enemy():
m_isDead(false),
m_damageFrame(0)
{
	
	
}

Enemy::~Enemy()
{

}

void Enemy::Init()
{

}
void Enemy::Update()
{
	switch (_state)
	{
	case  EnemyState::Damage:
		break;
	default:
		//重力、地面の判定、当たり判定の更新
		::Character::Update();
		break;
	}
	

}
void Enemy::Draw()
{
	//当たり判定の描画
	Character::Draw();
}

void Enemy::DamageUpdate()
{
	m_damageFrame++;
	//プレイヤー都の向きによって変える

	m_vel.x += 0.5f;
	m_pos += m_vel;
	if (m_damageFrame >= damageMaxFrame)
	{
		m_isDead = true;
	}

}

void Enemy::DropItem(std::shared_ptr<EnemyWizard> enemy)
{
	HitWizard = enemy;
	//SceneMainに渡してアイテムを探して出させる
}
void Enemy::DropItem(std::shared_ptr<EnemyRider> enemy)
{
	HitRider = enemy;
	//SceneMainに渡してアイテムを探して出させる
}
