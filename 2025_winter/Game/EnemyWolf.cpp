#include "EnemyWolf.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>


namespace
{
	const Vec2 kInitPos = { 1340.0f,800.0f };//初期位置
	constexpr float kSpeed = 3.0f;//移動速度
	constexpr int enemy_cut_w = 100;
	constexpr int enemy_cut_h = 100;
	constexpr float  enemy_scale = 5.0f;


	constexpr float kAttack1Speed = 12.0f;//攻撃時の速度
	constexpr float kAttack2Speed = 8.0f;//攻撃時の速度
	constexpr float kJumpSpeed = 20.0f;
	constexpr float kJumpDownSpeed = 15.0f;

	//被ダメージクールタイム
	constexpr int cool_interval = 60;


	//1frameあたりのアニメーションの時間
	constexpr int kIdleDuration = 6;
	constexpr int kWalkDuration = 8;
	constexpr int kAttackDuration = 6;
	constexpr int kAttack2Duration = 4;
	constexpr int kAttack3Duration = 4;
	constexpr int kDamageDuration = 20;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kIdleNum = 6;
	constexpr int kWalkNum = 8;
	constexpr int kAttackNum = 13;
	constexpr int kAttack2Num = 9;
	constexpr int kAttack3Num = 9;
	constexpr int kDamageNum = 4;



	constexpr float kAttackTime = 100.0f;  //攻撃の時間

	//攻撃1
	constexpr float kDashSpeed = 80.0f;
	constexpr float kThunderReadyTime = kAttackTime * 50 / 100;//攻撃前の見切り
	constexpr float kThunderDashTime = kAttackTime * 15 / 100;//攻撃中
	constexpr float kThunderEndTime = kAttackTime * 30 / 100;//攻撃後
	constexpr float kThunderAfterTime = kAttackTime * 5 / 100;//硬直中



	//攻撃2
	constexpr float kJumpAttackUpTime = kAttackTime * 2 / 10;
	constexpr float kJumpAttackStayTime = kAttackTime * 4 / 10;
	constexpr float kJumpAttackDownTime = kAttackTime * 3 / 10;
	constexpr float kJumpAttackFreeTime = kAttackTime * 1 / 10;

	//ダウンタイム
	constexpr float kDownTime = 100.0f;

	//左上限
	constexpr float kLeftMax = 90.0f;
	constexpr float kRightMax = 2000.0f;

}



EnemyWolf::EnemyWolf() :
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isAttack(false),
	isThrow(false),
	m_ready(false),
	m_wasReady(false),
	m_coolDamageTimer(0),
	m_hp(100),
	m_attackP(WolfAttackPattern::Attack3)
{
	m_handle = LoadGraph("data/Game/Werewolf.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_isRight = false;
	_state = EnemyState::Attack;
	coolTimer = coolTime * 1.5;
}

EnemyWolf::~EnemyWolf()
{
}

void EnemyWolf::Init()
{
}

void EnemyWolf::Update()
{
	m_animframe++;
	if (m_coolDamageTimer > 0)
	{
		m_coolDamageTimer--;
	}

	ClearAttackRect();
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
		return;
		break;
	default:
		break;
	}


	Character::BossUpdate();
}

void EnemyWolf::Draw()
{
}
void EnemyWolf::Draw(Camera& camera)
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
	case EnemyState::Damage://死亡演出
		charaIdx = (m_animframe / kDamageDuration) % kDamageNum;
		charaIdy = 5;
		if (m_animframe >= kDamageDuration * kDamageNum)
		{
			charaIdx = kDamageNum - 1;
		}
		drawY = 0;
		break;
	default:
		break;
	}




	if (m_coolDamageTimer > 0)
	{
		if ((m_coolDamageTimer / 10) % 2 == 0)SetDrawBright(255, 100, 100);//赤っぽく
	}


	if (m_attackP == WolfAttackPattern::Down)
	{
		if (charaIdx == 6)SetDrawBright(100, 100, 255);//青っぽく


		//stan用の動き
		if (attackTimer < kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime && (m_isGround && charaIdx == 6))//stay条件よりも低いとき、
		{
			//xとyで揺らし方を変える
			m_drawoffset.x = std::sin(m_animframe * 0.2f) * 3;
			m_drawoffset.y = std::cos(m_animframe * 0.15f) * 3 * 0.5f;//Yのほうを小さめにずらす
		}
		else
		{
			m_drawoffset = { 0,0 };
		}
	}
	else
	{
		m_drawoffset = { 0,0 };
	}



	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + drawX + m_drawoffset.x,
			m_pos.y + camera.drawOffset.y + drawY + m_drawoffset.y,
		enemy_cut_w * charaIdx, enemy_cut_h * charaIdy,//切り取り左上
		enemy_cut_w, enemy_cut_h,//切り取りの幅
		enemy_scale, 0.0f, m_handle, true, false);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + drawX + m_drawoffset.x,
			m_pos.y + camera.drawOffset.y + drawY + m_drawoffset.y,
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

	m_attack1Rect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);

	//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - catchDistance + camera.drawOffset.x, 0, m_pos.x + catchDistance + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);

	/*printf("Frame: attackTimer = %f, m_pos.x = %f, m_vel.x = %f\n",
		   attackTimer, m_pos.x, m_vel.x);*/

#endif // DEBUG

}
void EnemyWolf::HitBossDamage(int damage)
{
	if (m_coolDamageTimer > 0)return;

	//死んでるアニメーションの時は、被ダメしない
	if (_state == EnemyState::Damage)return;

	m_hp -= damage;
	m_coolDamageTimer = cool_interval;
	if (m_hp <= 0)
	{
		m_hp = 0;

		_state = EnemyState::Damage;
		m_vel = Vec2{ 0,0 };
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
}

void EnemyWolf::AnimChange(const EnemyState state)
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

void EnemyWolf::NormalUpdate()
{
}

void EnemyWolf::WalkUpdate()
{
	m_vel.x = m_isRight ? kSpeed : -kSpeed;

}
void EnemyWolf::AttackUpdate()
{
	switch (m_attackP)
	{
	case WolfAttackPattern::Attack1:
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
				//dir ? m_vel.x = kAttack1Speed : m_vel.x = -kAttack1Speed;

				m_isRight = dir;
				//アニメーションを初期化
				m_animframe = 0;
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
	case WolfAttackPattern::Attack2:
	{
		coolTimer--;

		//左上限をきめる
		if (m_pos.x <= kLeftMax)
		{
			m_pos.x = kLeftMax;
		}
		//右上限を決める
		if (m_pos.x >= kRightMax)
		{
			m_pos.x = kRightMax;
		}


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
				//アニメーションを初期化
				m_animframe = 0;
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
	case WolfAttackPattern::Attack3:
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
				//アニメーションを初期化
				m_animframe = 0;
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
	case WolfAttackPattern::Down://波動
	{
		coolTimer--;

		//左上限をきめる
		if (m_pos.x <= kLeftMax)
		{
			m_pos.x = kLeftMax;
		}
		//右上限を決める
		if (m_pos.x >= kRightMax)
		{
			m_pos.x = kRightMax;
		}

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
				//アニメーションを初期化
				m_animframe = 0;
			}
		}
		if (isAttack == true)
		{
			//敵はプレイヤーの向きに攻撃する
			//攻撃終わり処理も入っている
			AttackDown();

		}
	
	}
	break;

	}


}

void EnemyWolf::DamageUpdate()
{
	//死亡アニメーションをする

	if (m_animframe == kDamageDuration * kDamageNum)
	{
		isDead = true;

		//エフェクトを出す
		for (auto& func : onDeathEvents)
		{
			if (func)func();//呼び出し
		}
	}

	//重力処理
	Gravity();




	m_colRect.SetLT(0, 5000, 0, 0);
	Rect chipRect;//当たったマップチップの矩形
	CheckHitMap(chipRect);
	m_colRect.SetLT(0, 5000, 0, 0);

	if (m_isGround)
	{

		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;


	}

}

void EnemyWolf::AttackAnimIdxy()
{
	float drawX = 0;
	float drawY = 0;

	switch (m_attackP)
	{
	case WolfAttackPattern::Attack1:
	{
		if (isAttack == true)
		{
			charaIdx = (m_animframe / kAttackDuration) % kAttackNum;
			charaIdy = 3;
			drawY += enemy_cut_h / 4;

			//ここから条件分岐
			if (attackTimer > kAttackTime - kThunderReadyTime)//最初のフレームはロックオン
			{
				//初めて、3fになっているときは光のエフェクトを出す
				
				//3fまで
				if (charaIdx >= 3)
				{
					charaIdx = 3;
				}
			}
			else if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime)//ダッシュ中
			{
				baseFrame = 4; // フレーム4から開始
				phaseProgress = ((kAttackTime - kThunderReadyTime) - attackTimer);//前の分岐条件からの数えだしだから0スタート
				charaIdx = baseFrame + (phaseProgress / kAttackDuration);
				//7fまで
				if (charaIdx >= 7)
				{
					charaIdx = 7;
				}
			
			}
			else //if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime - kThunderEndTime)//あと残り
			{
				//最後の13fまで
				baseFrame = 8;
				phaseProgress = ((kAttackTime - kThunderReadyTime - kThunderDashTime) - attackTimer);
				charaIdx = baseFrame + (phaseProgress / kAttackDuration);
				if (charaIdx >= kAttackNum-1)
				{
					charaIdx = kAttackNum-1;
				}

			}
			//else if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime - kThunderEndTime - kThunderAfterTime)//攻撃硬直
			//{
			//	
			//}

		}
		else//Idle状態にする
		{
			charaIdx = (m_animframe / kIdleDuration) % kIdleNum;
			charaIdy = 0;
			drawY -= enemy_cut_h / 2;
		}
	}
	break;
	case WolfAttackPattern::Attack2:
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
	case WolfAttackPattern::Attack3:
	{
		if (isAttack == true)
		{
			charaIdx = (m_animframe / kAttack3Duration) % kAttack3Num;
			charaIdy = 2;
			drawY += enemy_cut_h / 4;
			if (attackTimer > kAttackTime - kThunderReadyTime)//最初のフレームはロックオン
			{
				charaIdx = (m_animframe / kAttackDuration) % kAttackNum;
				charaIdy = 3;
				drawY += enemy_cut_h / 4;
				//初めて、3fになっているときは光のエフェクトを出す

				//3fまで
				if (charaIdx >= 3)
				{
					charaIdx = 3;
				}

				
			}
			else
			{
				charaIdx = (m_animframe / kAttack3Duration) % kAttack3Num;
				charaIdy = 2;
				drawY += enemy_cut_h / 4;
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
	case WolfAttackPattern::Down:
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
				charaIdx = baseFrame + (phaseProgress / kAttack2Duration / 2);//kAttack2Durationの2倍の速さで進む
				if (charaIdx > 6) charaIdx = 6;
			}
			else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime - kJumpAttackFreeTime)
			{
				charaIdx = 6;
			}
			else
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


}

void EnemyWolf::Attack1()
{
	//実際に攻撃をする処理
	attackTimer--;

	if (attackTimer > kAttackTime - kThunderReadyTime)//最初のフレームはロックオン
	{
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		//dir ? m_vel.x = kAttack1Speed : m_vel.x = -kAttack1Speed;

		m_isRight = dir;
		float offset = 100.0f;
		//攻撃終わり位置を決める
		if (m_isRight)
		{
			m_DashEnd.x = m_pPlayer->GetPos().x + offset;
		}
		else
		{
			m_DashEnd.x = m_pPlayer->GetPos().x - offset;
		}
	}
	else if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime)//ダッシュ中
	{
		if (m_isRight)
		{
			m_vel.x = kDashSpeed;
		}
		else
		{
			m_vel.x = -kDashSpeed;
		}
		
		//斬撃判定をつける
		m_attack1Rect.SetLT(m_pos.x + (m_isRight ? 50.0f : -160.0f), m_pos.y - enemy_cut_w/2, 110.0f, 100.0f);
	}
	else if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime- kThunderEndTime)//あと残り
	{
		
		m_vel.x = 0;
		//エフェクトなど
	}
	else if (attackTimer > kAttackTime - kThunderReadyTime - kThunderDashTime - kThunderEndTime - kThunderAfterTime)//攻撃硬直
	{
		//攻撃硬直
	}


	

	//charaIdxが3fになったら
	//キャラを瞬間移動させる、アニメーションを進ませる

	

	if (attackTimer <= 0)
	{
		//_state = EnemyState::Normal;
		//attackTimer = kAttackTime;
		isAttack = false;
		m_animframe = 0;//アニメーション用
		m_vel = zero;
		coolTimer = coolTime;

		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;

		//次の攻撃パターンを選択
		{
			WolfAttackPattern prev = m_attackP;
			do
			{
				m_attackP = SelectAttack();
			} while (m_attackP == prev);
		}


	}
}

void EnemyWolf::Attack2()
{
	attackTimer--;



	if (attackTimer > kAttackTime - kJumpAttackUpTime)//最初の上昇
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

	if (attackTimer == 10)//着地の瞬間
	{
		//エフェクトを出す
		for (auto& func : onAttackEndEvents)
		{
			if (func)func();//呼び出し
		}
	}


	if (attackTimer <= 0)
	{
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;
		m_animframe = 0;//アニメーション用


		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;

		//次の攻撃パターンを選択
		{
			WolfAttackPattern prev = m_attackP;
			do
			{
				m_attackP = SelectAttack();
			} while (m_attackP == prev);
		}
	}
}

void EnemyWolf::Attack3()
{
	attackTimer--;
	//アニメーション関連
	m_wasReady = m_ready;
	if (attackTimer <= kAttackTime - kThunderReadyTime)
	{
		m_ready = true;
	}
	if (m_ready && !m_wasReady)
	{
		m_animframe = 0;//アニメーション用
	}

	if (attackTimer > kAttackTime - kThunderReadyTime)//最初のフレームはロックオン
	{
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		//dir ? m_vel.x = kAttack1Speed : m_vel.x = -kAttack1Speed;

		m_isRight = dir;
	}
	

	if (charaIdx ==4 || charaIdx == 5|| charaIdx == 6)//攻撃判定出すフレーム
	{
		if (m_ready)
		{
			//めんどくさいから攻撃1の判定
		//攻撃判定
			m_attack1Rect.SetLT(m_pos.x + (m_isRight ? 50.0f : -160.0f), m_pos.y - enemy_cut_w / 2, 110.0f, 100.0f);
		}
		
	}
	

	if (attackTimer <= 0)
	{
		isAttack = false;
		m_ready = false;
		m_wasReady = false;
		m_animframe = 0;//アニメーション用
		m_vel = zero;
		coolTimer = coolTime;
		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;

		
				m_attackP = SelectAttack();
	

	}
}

void EnemyWolf::AttackDown()
{
	attackTimer--;



	if (attackTimer > kAttackTime - kJumpAttackUpTime)//最初の上昇
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
		m_vel.y += kJumpDownSpeed * 1.5f;
	}
	else if (attackTimer > kAttackTime - kJumpAttackUpTime - kJumpAttackStayTime - kJumpAttackDownTime - kJumpAttackFreeTime)//後隙
	{
		m_vel.y += kJumpDownSpeed * 1.5f;
	}

	//着地の瞬間にカメラを揺らす処理




	if (attackTimer <= 0 - kDownTime)
	{
		isAttack = false;
		m_vel = zero;
		coolTimer = coolTime;



		//プレイヤーのほうをむく
		bool  dir = m_pPlayer->GetPos().x > m_pos.x;
		m_isRight = dir;

		//次の攻撃パターンを選択
		{
			WolfAttackPattern prev = m_attackP;
			do
			{
				m_attackP = SelectAttack();
			} while (m_attackP == prev);
		}

	}
}

WolfAttackPattern EnemyWolf::SelectAttack()
{

	//アニメーションの初期化
	m_animframe = 0;//アニメーション用

	int flow = 0;
	//--------------
	int fmax = 70;
	int smax = 44;
	int thmax = 95;
	//---------------
	int fomax = 100;


	int ans = GetRand(99);//0から99の100通り
	if (0 <= ans && ans < fmax)
	{
		return WolfAttackPattern::Attack1;
	}
	if (fmax <= ans && ans < fomax)
	{
		return WolfAttackPattern::Attack3;
	}


}

void EnemyWolf::ClearAttackRect()
{
	m_attack1Rect.SetLT(0, 0, 0, 0);
}

