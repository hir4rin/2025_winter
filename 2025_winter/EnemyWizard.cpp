#include "EnemyWizard.h"
#include "DxLib.h"
#include "Bg.h"

namespace
{
	const Vec2 kInitPos = { 1000.0f,100.0f };//初期位置
	constexpr float kSpeed = 2.0f;//移動速度
	constexpr int enemy_cut_w = 64;
	constexpr int enemy_cut_h = 64;
	constexpr float  enemy_scale = 3.0f;


	constexpr float kAttackSpeed = 0.0f;//攻撃時の速度
	float attackTime = 15.f;  //攻撃の時間
	float attackTimer = 0.0f;//攻撃を計るタイマー
}


EnemyWizard::EnemyWizard():
	charaIdx(0),
	charaIdy(0),
	m_animframe(0)
{
	m_handle = LoadGraph("data/Penguin.png");
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
		AnimChange(EnemyState::Attack);
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
		case EnemyState::Normal:
		charaIdx = (m_animframe / 5) % 2;
		charaIdy = 0;
		drawY -= enemy_cut_h / 2;
		break;
		case EnemyState::Walk:
			charaIdx = (m_animframe / 10) % 6;
			charaIdy = 3;
			drawY += enemy_cut_h / 3 * 2;
			break;
		case EnemyState::Attack:
			charaIdx = (m_animframe / 5) % 7;
			charaIdy = 2;
			drawY += enemy_cut_h / 4;
			break;
		case EnemyState::Damage:
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
}

void EnemyWizard::AnimChange(const EnemyState state)
{
	//攻撃モーション終了条件

	if (_state == EnemyState::Attack)return;

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
	//プレイヤーのポインタをSceneMainからもらう必要があるところ

}
void EnemyWizard::Attack()
{
	//実際に攻撃をする処理
	attackTimer--;
	if (attackTimer <= 0)
	{
		_state = EnemyState::Normal;
		attackTimer = attackTime;
	}
}


