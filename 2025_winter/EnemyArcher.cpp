#include "EnemyArcher.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"


namespace
{
	const Vec2 kInitPos = { 1800.0f,100.0f };//初期位置
	constexpr float kSpeed = 2.0f;//移動速度
	constexpr int enemy_cut_w = 100;
	constexpr int enemy_cut_h = 100;
	constexpr float  enemy_scale = 3.0f;


	constexpr float kAttackSpeed = 3.0f;//攻撃時の速度
	float attackTime = 90.0f;  //攻撃の時間
	float attackTimer = 0.0f;//攻撃を計るタイマー

	float coolTimer = 0.0f;//クールダウンを図るタイマー
	float coolTime = 180.0f;//クールタイム

	float catchDistance = 400.0f;//プレイヤーを見つける距離
}



EnemyArcher::EnemyArcher():
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isAttack(false)
{
	m_handle = LoadGraph("data/EnemyArcher.png");
	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Walk;
}

EnemyArcher::~EnemyArcher()
{
}

void EnemyArcher::Init()
{
}

void EnemyArcher::Update()
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

/// <summary>
/// 使わない
/// </summary>
void EnemyArcher::Draw()
{
}
void EnemyArcher::Draw(Camera& camera)
{
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
	float drawX = 0;
	float drawY = 0;

	switch (_state)
	{
	case EnemyState::Normal://Idle
		charaIdx = (m_animframe / 8) % 6;
		charaIdy = 0;
		drawY += enemy_cut_h / 8;
		break;
	case EnemyState::Walk://Walk
		charaIdx = (m_animframe / 10) % 8;
		charaIdy = 1;
		drawY += enemy_cut_h / 8;
		break;
	case EnemyState::Attack://Attack
		if (isAttack == true)
		{
			charaIdx = (m_animframe / 5) % 9;
			charaIdy = 2;
			drawY += enemy_cut_h / 8;
		}
		else//Idle状態にする
		{
			charaIdx = (m_animframe / 8) % 6;
			charaIdy = 0;
			drawY += enemy_cut_h / 8;
		}

		break;
	case EnemyState::Damage://Damage
		break;
	default:
		break;
	}
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + drawX,
			m_pos.y + camera.drawOffset.y + drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true, false);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + drawX,
			m_pos.y + camera.drawOffset.y + drawY,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f,
			m_handle, true, true);
	}

	//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - catchDistance / 2 + camera.drawOffset.x, 0, m_pos.x + catchDistance / 2 + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);
}

void EnemyArcher::AnimChange(const EnemyState state)
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

void EnemyArcher::NormalUpdate()
{
}

void EnemyArcher::WalkUpdate()
{
	m_vel.x = -kSpeed;

}
void EnemyArcher::AttackUpdate()
{

	coolTimer--;
	//ぷれいやーのとの距離が近くなったら攻撃を出す
	float distance = m_pPlayer->GetPos().x - m_pos.x;
	if (distance < 0)distance = -distance;//絶対値にする
	if (distance < catchDistance && !isAttack)
	{
		if (coolTimer <= 0)//クールタイムが0になったら
		{
			isAttack = true;
			attackTimer = attackTime;
			AnimChange(EnemyState::Attack);
			//弓矢が移動するかはまだ検討中
			/*bool  dir = m_pPlayer->GetPos().x > m_pos.x;
			dir ? m_vel.x = kAttackSpeed : m_vel.x = -kAttackSpeed;*/
		}
	}
	if (isAttack == true)
	{
		//敵はプレイヤーの向きに攻撃する

		Attack();

	}

}
void EnemyArcher::Attack()
{
	//実際に攻撃をする処理
	attackTimer--;
	if (attackTimer <= 0)
	{
		//_state = EnemyState::Normal;
		attackTimer = attackTime;
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;
	}
}

