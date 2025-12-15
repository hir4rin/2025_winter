#include "EnemyWizard.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	const Vec2 kInitPos = { 000.0f,000.0f };//初期位置
	constexpr float kSpeed = 3.0f;//移動速度
	constexpr int enemy_cut_w = 64;
	constexpr int enemy_cut_h = 64;
	constexpr float  enemy_scale = 3.0f;


	constexpr float kAttackSpeed = 3.0f;//攻撃時の速度

}


EnemyWizard::EnemyWizard():
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isAttack(false)
{
	m_handle = LoadGraph("data/Game/Penguin.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Walk;
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
	
	switch (_state)
	{
	case EnemyState::Normal:
		AnimChange(EnemyState::Normal);
		//Normalのアップデート
		NormalUpdate();
		break;
	case EnemyState::Walk:
		AnimChange(EnemyState::Walk);
		//Walkのアップデート
		WalkUpdate();
		break;
	case EnemyState::Attack:
		//Attackのアップデート
		AttackUpdate();
		break;
	case EnemyState::Damage:
		//DamageのUpdate
		DamageUpdate();
		break;
	default:
		break;
	}
	
		
	Enemy::Update();
	
}

void EnemyWizard::Draw()
{
	

}
void EnemyWizard::Draw(Camera& camera)
{
	
	
	float drawX = 0;
	float drawY = 0;

	switch (_state)
	{
	case EnemyState::Normal://Idle
		charaIdx = (m_animframe / 8) % 2;
		charaIdy = 0;
		drawY -= enemy_cut_h / 2;
		break;
	case EnemyState::Walk://Walk
			charaIdx = (m_animframe / 10) % 6;
			charaIdy = 3;
			drawY += enemy_cut_h / 3 * 2;
			break;
	case EnemyState::Attack://Attack
			if (isAttack == true)
			{
				charaIdx = (m_animframe / 5) % 7;
				charaIdy = 2;
				drawY += enemy_cut_h / 4;
			}
			else//Idle状態にする
			{
				charaIdx = (m_animframe / 8) % 2;
				charaIdy = 0;
				drawY -= enemy_cut_h / 2;
			}
			
			break;
	case EnemyState::Damage://Damage(一旦ノーマルのやつ)
		charaIdx = (m_animframe / 8) % 2;
		charaIdy = 0;
		drawY -= enemy_cut_h / 2;
			break;
		default:
			break;
	}
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x+ drawX,
			m_pos.y + camera.drawOffset.y+ drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true,true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x+ drawX,
			m_pos.y + camera.drawOffset.y + drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, 
			m_handle, true, false);
	}

#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
	//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - catchDistance + camera.drawOffset.x, 0, m_pos.x + catchDistance + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);

#endif // DEBUG

}

void EnemyWizard::AnimChange(const EnemyState state)
{
	
	if (isAttack)return;

	if (_state != state)
	{
		_state = state;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
	
}

void EnemyWizard::NormalUpdate()
{
}

void EnemyWizard::WalkUpdate()
{
	m_vel.x = m_isRight ? kSpeed : -kSpeed;
	
}
void EnemyWizard::AttackUpdate()
{
	coolTimer--;
	//ぷれいやーのとの距離が近くなったら攻撃を出す
	float distance = m_pPlayer->GetPos().x - m_pos.x;
	if (distance < 0)distance = -distance;//絶対値にする
	if (distance < catchDistance && !isAttack)
	{
		if (coolTimer <= 0)//クールタイムが0になったら
		{
			AnimChange(EnemyState::Attack);
		isAttack = true;
		attackTimer = attackTime;
		
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		dir ? m_vel.x = kAttackSpeed : m_vel.x = -kAttackSpeed;

		m_isRight = dir;
		}
	}
	if (isAttack == true)
	{
		//敵はプレイヤーの向きに攻撃する
		
		Attack();
		
	}

}

void EnemyWizard::DamageUpdate()
{
	Enemy::DamageUpdate();

}

void EnemyWizard::Attack()
{
	//実際に攻撃をする処理
	attackTimer--;
	if (attackTimer <= 0)
	{
		//_state = EnemyState::Normal;
		//attackTimer = attackTime;
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;
	}
}


