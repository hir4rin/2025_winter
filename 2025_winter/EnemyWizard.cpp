#include "EnemyWizard.h"
#include "DxLib.h"
#include "Bg.h"
#include "Player.h"

namespace
{
	const Vec2 kInitPos = { 1000.0f,100.0f };//初期位置
	constexpr float kSpeed = 2.0f;//移動速度
	constexpr int enemy_cut_w = 64;
	constexpr int enemy_cut_h = 64;
	constexpr float  enemy_scale = 3.0f;


	constexpr float kAttackSpeed = 3.0f;//攻撃時の速度
	float attackTime = 30.f;  //攻撃の時間
	float attackTimer = 0.0f;//攻撃を計るタイマー

	float catchDistance = 200.0f;//プレイヤーを見つける距離
}


EnemyWizard::EnemyWizard():
	charaIdx(0),
	charaIdy(0),
	m_animframe(0)
{
	m_handle = LoadGraph("data/Penguin.png");
	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Attack;
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
	default:
		break;
	}

}

void EnemyWizard::Draw()
{
	float drawX = m_pos.x - m_pBg->GetScrollX();
	float drawY = m_pos.y - m_pBg->GetScrollY();

	//当たり判定の描画
	Character::Draw();
	DrawFormatString(100, 150, GetColor(255, 0, 0), "敵のposは%5fです", m_pos.x);


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
	case EnemyState::Damage://Damage
			break;
		default:
			break;
	}
	if (m_isRight)
	{
		DrawRectRotaGraph(drawX, drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true,true);
	}
	else
	{
		DrawRectRotaGraph(drawX, drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true, false);
	}

	//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x + catchDistance, 0, m_pos.x - catchDistance, 1080, GetColor(0, 0, 255), false);
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
	m_vel.x = -kSpeed;
	
}
void EnemyWizard::AttackUpdate()
{
	//ぷれいやーのとの距離が近くなったら攻撃を出す
	float distance = m_pPlayer->GetPos().x - m_pos.x;
	if (distance < 0)distance = -distance;//絶対値にする
	if (distance < catchDistance)
	{
		isAttack = true;
		attackTimer = attackTime;
		AnimChange(EnemyState::Attack);
	}
	if (isAttack == true)
	{
		m_vel.x = -kAttackSpeed;
		Attack();
		
	}

}
void EnemyWizard::Attack()
{
	//実際に攻撃をする処理
	attackTimer--;
	if (attackTimer <= 0)
	{
		//_state = EnemyState::Normal;
		attackTimer = attackTime;
		isAttack = false;
		m_vel = zero;
	}
}


