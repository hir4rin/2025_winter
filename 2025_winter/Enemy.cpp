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

}

Enemy::Enemy()
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
	//重力、地面の判定、当たり判定の更新
	::Character::Update();

}
void Enemy::Draw()
{
	//当たり判定の描画
	Character::Draw();
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
