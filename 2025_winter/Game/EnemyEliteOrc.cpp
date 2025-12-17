#include "EnemyEliteOrc.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>


namespace
{
	const Vec2 kInitPos = { 3330.0f,800.0f };//初期位置
	constexpr float kSpeed = 3.0f;//移動速度
	constexpr int enemy_cut_w = 100;
	constexpr int enemy_cut_h = 100;
	constexpr float  enemy_scale = 5.0f;


	constexpr float kAttack1Speed = 6.0f;//攻撃時の速度
	constexpr float kAttack2Speed = 8.0f;//攻撃時の速度
	constexpr float kJumpSpeed = 20.0f;
	constexpr float kJumpDownSpeed = 15.0f;

	//被ダメージクールタイム
	constexpr int cool_interval = 60;

	
//1frameあたりのアニメーションの時間
	constexpr int kIdleDuration = 6;
	constexpr int kWalkDuration = 8;
	constexpr int kAttackDuration = 2;
	constexpr int kAttack2Duration = 4;
	constexpr int kAttack3Duration = 12;
	constexpr int kDamageDuration = 10;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kIdleNum = 6;
	constexpr int kWalkNum = 8;
	constexpr int kAttackNum = 9;
	constexpr int kAttack2Num = 9;
	constexpr int kAttack3Num = 7;
	constexpr int kDamageNum = 4;



	constexpr float kAttackTime = 100.0f;  //攻撃の時間


	//攻撃2
	constexpr float kJumpAttackUpTime = kAttackTime * 2/10;
	constexpr float kJumpAttackStayTime = kAttackTime  * 4/10;
	constexpr float kJumpAttackDownTime = kAttackTime * 3/10;
	constexpr float kJumpAttackFreeTime = kAttackTime * 1/10;

}


EnemyEliteOrc::EnemyEliteOrc() :
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isAttack(false),
	isThrow(false),
	m_coolDamageTimer(0),
	m_hp(100),
	m_attackP(AttackPattern::Attack3)
{
	m_handle = LoadGraph("data/Game/Elite Orc.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Attack;
}

EnemyEliteOrc::~EnemyEliteOrc()
{
}

void EnemyEliteOrc::Init()
{
}

void EnemyEliteOrc::Update()
{
	m_animframe++;
	if (m_coolDamageTimer > 0)
	{
		m_coolDamageTimer--;
	}

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


	Character::BossUpdate();

}

void EnemyEliteOrc::Draw()
{


}
void EnemyEliteOrc::Draw(Camera& camera)
{


	float drawX = 0;
	float drawY = 0;

	switch (_state)
	{
	case EnemyState::Normal://Idle
		charaIdx = (m_animframe / kIdleDuration) % kIdleNum;
		charaIdy = 0;
		drawY -= enemy_cut_h / 2;
		break;
	case EnemyState::Walk://Walk
		charaIdx = (m_animframe / kWalkDuration) % kWalkNum;
		charaIdy = 1;
		drawY += enemy_cut_h / 3 * 2;
		break;
	case EnemyState::Attack://Attack
		
		AttackAnimIdxy();

		break;
	case EnemyState::Damage://Damage(一旦ノーマルのやつ)
		charaIdx = (m_animframe / kDamageDuration) % kDamageNum;
		charaIdy = 5;
		drawY -= enemy_cut_h / 2;
		break;
	default:
		break;
	}

	if (m_coolDamageTimer > 0)
	{
		if ((m_coolDamageTimer /10) % 2 == 0)SetDrawBright(255, 100, 100);//赤っぽく
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
	//元に戻す
	SetDrawBright(255, 255, 255);


#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
	//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - catchDistance + camera.drawOffset.x, 0, m_pos.x + catchDistance + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);

	/*printf("Frame: attackTimer = %f, m_pos.x = %f, m_vel.x = %f\n",
		   attackTimer, m_pos.x, m_vel.x);*/

#endif // DEBUG

}

void EnemyEliteOrc::HitBossDamage(int damage)
{
	if (m_coolDamageTimer > 0)return;

	m_hp -= damage;
	m_coolDamageTimer = cool_interval;
	if (m_hp <= 0)
	{
		m_hp = 0;
		isDead = true;
	}
}

void EnemyEliteOrc::AnimChange(const EnemyState state)
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

void EnemyEliteOrc::NormalUpdate()
{
}

void EnemyEliteOrc::WalkUpdate()
{
	m_vel.x = m_isRight ? kSpeed : -kSpeed;

}
void EnemyEliteOrc::AttackUpdate()
{
	switch (m_attackP)
	{
	case AttackPattern::Attack1:
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
				attackTimer = kAttackTime;

				bool  dir = m_pPlayer->GetPos().x > m_pos.x;
				dir ? m_vel.x = kAttack1Speed : m_vel.x = -kAttack1Speed;

				m_isRight = dir;
			}
		}
		if (isAttack == true)
		{
			//敵はプレイヤーの向きに攻撃する
			//攻撃終わり処理
			Attack1();

		}
	}
		break;
	case AttackPattern::Attack2:
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
				attackTimer = kAttackTime;

				bool  dir = m_pPlayer->GetPos().x > m_pos.x;
				m_isRight = dir;

				m_startX = m_pos.x;//減速処理用

				if (m_isRight)
				{
					targetX = m_pPlayer->GetPos().x + offset;
				}
				else
				{
					targetX = m_pPlayer->GetPos().x - offset;
				}
				float distance = targetX - m_pos.x;
				m_vel.x = distance / kJumpAttackUpTime;

				m_vel.y = -kJumpSpeed;
			}
		}
		if (isAttack == true)
		{
			//敵はプレイヤーの向きに攻撃する
			//攻撃終わり処理も入っている
			Attack2();

		}
	}
		break;
	case AttackPattern::Attack3:
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
				attackTimer = kAttackTime;

				bool  dir = m_pPlayer->GetPos().x > m_pos.x;
				m_isRight = dir;
			}
		}
		if (isAttack == true)
		{
			//敵はプレイヤーの向きに攻撃する
			//攻撃終わり処理も入っている
			Attack3();

		}
	}
		break;

	}
	

}

void EnemyEliteOrc::DamageUpdate()
{
	Enemy::DamageUpdate();

}

void EnemyEliteOrc::AttackAnimIdxy()
{
	float drawX = 0;
	float drawY = 0;

	switch (m_attackP)
	{
	case AttackPattern::Attack1:
	{
		if (isAttack == true)
		{
			charaIdx = (m_animframe / kAttackDuration) % kAttackNum + 1;
			charaIdy = 3;
			drawY += enemy_cut_h / 4;
		}
		else//Idle状態にする
		{
			charaIdx = (m_animframe / kIdleDuration) % kIdleNum;
			charaIdy = 0;
			drawY -= enemy_cut_h / 2;
		}
	}
		break;
	case AttackPattern::Attack2:
	{
		if (isAttack == true)
		{
			charaIdx = (m_animframe / kAttack2Duration) % kAttack2Num;
			charaIdy = 4;
			drawY += enemy_cut_h / 4;

			if (attackTimer > kAttackTime - kJumpAttackUpTime)//最初の上昇
			{
				if (charaIdx >= 2)charaIdx = 2;

			}
			else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime)//真ん中のstayTime
			{
				charaIdx = 3;
			}
			else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime)
			{
				baseFrame = 4; // フレーム4から開始
				//このフレームから(0)の進み具合
				phaseProgress = ((kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime) - attackTimer);
				charaIdx = baseFrame + (phaseProgress / kAttack2Duration);
				if (charaIdx > 6) charaIdx = 6;
			}
			else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime - kJumpAttackFreeTime)
			{
				charaIdx = 6;
			}

		}
		else//Idle状態にする
		{
			charaIdx = (m_animframe / kIdleDuration) % kIdleNum;
			charaIdy = 0;
			drawY -= enemy_cut_h / 2;
		}
	}
		break;
	case AttackPattern::Attack3:
	{
		if (isAttack == true)
		{
			charaIdx = (m_animframe / kAttack3Duration) % kAttack3Num;
			charaIdy = 2;
			drawY += enemy_cut_h / 4;
		}
		else//Idle状態にする
		{
			charaIdx = (m_animframe / kIdleDuration) % kIdleNum;
			charaIdy = 0;
			drawY -= enemy_cut_h / 2;
		}
	}
		break;
	}

	
}

void EnemyEliteOrc::Attack1()
{
	//実際に攻撃をする処理
	attackTimer--;
	if (attackTimer <= 0)
	{
		//_state = EnemyState::Normal;
		//attackTimer = kAttackTime;
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;

		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;
	}
}

void EnemyEliteOrc::Attack2()
{
	attackTimer--;
	
	

	if (attackTimer > kAttackTime-kJumpAttackUpTime)//最初の上昇
	{
		m_vel.y += -kJumpSpeed;//途中のジャンプ力

		//手前で止まる処理
		{
			//float distance = targetX - m_pos.x;
		//float moveSpeed = distance / kJumpAttackUpTime;//上昇時間内に到達
		//m_vel.x = moveSpeed;
		}
		
		//減速処理(ClearSceneのと一緒)
		{
			////全体の進捗度合い
			//float progress = (m_pos.x - m_startX) / (targetX - m_startX);
			////速度の割合(2乗を使って滑らか) y=1-x*x
			//float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに(イージング関数というやつらしい)(ググればわかる
			//if (m_isRight)
			//{
			//	m_vel.x = kAttack2Speed * 3 * speedRate;

			//}
			//else
			//{
			//	m_vel.x = -kAttack2Speed * 3 * speedRate;
			//}
		}
		
	}
	else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime)//真ん中のstayTime
	{
		m_vel.y = 0;//速度滞空
		m_vel.x = 0;
	}
	else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime)//0以上
	{
		m_vel.y += kJumpDownSpeed;//落下(重力+α)
	}
	else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime - kJumpAttackFreeTime)//後隙
	{
		m_vel.y += kJumpDownSpeed;
	}



	if (attackTimer <= 0)
	{
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;
		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;
	}
}

void EnemyEliteOrc::Attack3()
{
	attackTimer--;
	

	if (charaIdx == 3 && m_animframe % kAttack3Duration == 0)//3コマ目の最初に玉を投げる
	{
		isThrow = true;
	}
	else
	{
		isThrow = false;
	}

	if (attackTimer <= 0)
	{
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;
		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;

	}
}



