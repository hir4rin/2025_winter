#include "Fish.h"
#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include "Application.h"

namespace
{
	//スケール
	constexpr float kScale = 2.0f;
	//サイズ
	constexpr int kCharaWidth = 96;
	constexpr int kCharaHeight = 80;
	//スペシャルフィッシュの時
	constexpr int kSPWidth = 100;
	constexpr int kSPHeight = 100;
	//歩くspeed
	constexpr float kSpeed = 5.0f;
	//現れるときの演出時間
	constexpr int InitTime = 60;
	//バックする時間
	constexpr int backTime = 80;
	//ジャンプする時間
	constexpr int jumpTime = 15;
	//バックのクールタイム
	constexpr int backCoolTime = 100;
	
	//緑と青がアクションを起こすプレイヤーとの距離
	constexpr float kActionDistance = 200.0f;



	//地面都の描画関連
	//地面
	float groundY = 800.0f;
	//見えるライン
	float visibleTop = groundY;
	
	//切り取り操作の部分
	float srcY = kCharaHeight;

	//描画のいろいろ
	constexpr int kIdleDuration = 3;
	constexpr int kWalkDuration = 5;
	constexpr int kDeadDuration = 5;


	constexpr int kIdleNum = 14;
	constexpr int kWalkNum = 6;
	constexpr int kDeadNum = 7;

	constexpr int kIdleNumY = 0;
	constexpr int kWalkNumY = 1;
	constexpr int kDeadNumY = 6;

	//被ダメージクールタイム
	constexpr int cool_interval = 40;
}

Fish::Fish(Vec2 pos,int num):
	m_animFrame(0),
	m_num(num),
	m_firstFrame(0),
	m_isBack(false),
	m_backCoolTime(0),
	backFrame(0),
	m_angle(0),
	bossTimer(0),
	 m_coolDamageTimer(0),
	m_state(FishState::First)
{
	switch (m_num)
	{
	case 1:
		m_handle = LoadGraph("data/Game/Fish1.png");
		break;
	case 2:
		m_handle = LoadGraph("data/Game/Fish2.png");
		break;
	case 3:
		m_handle = LoadGraph("data/Game/Fish3.png");
		break;
	case 4:
		m_handle = LoadGraph("data/Game/Fish4.png");
		break;
	}

	m_pos = pos;
	m_hp = 150;

	m_colRect.SetCenter(1000, 0, 0, 0);
}

Fish::Fish(Vec2 pos, int num, int hp, FishState state):
	m_animFrame(0),
	m_num(num),
	m_firstFrame(0),
	m_isBack(false),
	m_backCoolTime(0),
	backFrame(0),
	m_angle(0),
	m_coolDamageTimer(0),
	bossTimer(0)
{
	switch (m_num)
	{
	case 1:
		m_handle = LoadGraph("data/Game/Fish1.png");
		break;
	case 2:
		m_handle = LoadGraph("data/Game/Fish2.png");
		break;
	case 3:
		m_handle = LoadGraph("data/Game/Fish3.png");
		break;
	case 4:
		m_handle = LoadGraph("data/Game/Fish4.png");
		break;
	}

	m_pos = pos;
	m_hp = hp;
	m_state = state;

	m_colRect.SetCenter(1000, 0, 0, 0);

	if (m_state == FishState::Release)
	{
		switch (m_num)
		{
		case 1:
			m_vel.x = -6.0f;
			break;
		case 2:
			break;
		case 3:
			m_vel.x = 6.0f;
			break;
		case 4:
			break;
		}
	}
	if (m_state == FishState::Dead)
	{
		switch (m_num)
		{
		case 1:
			m_vel.x = -5.0f;
			break;
		case 2:
			break;
		case 3:
			m_vel.x = 5.0f;
			break;
		case 4:
			break;
		}
	}
}

Fish::~Fish()
{
	DeleteGraph(m_handle);
}

void Fish::Init()
{

}

void Fish::Update()
{
	



	m_animFrame++;

	if (m_coolDamageTimer >= 0)
	{
		m_coolDamageTimer--;
	}
	

	switch (m_state)
	{
	case FishState::First:
	{
		m_firstFrame++;
		m_colRect.SetCenter(1000, 0, 0, 0);
		m_pos.y -= 1.5f;
		if (m_firstFrame > InitTime)
		{
			m_firstFrame = 0;
			m_animFrame = 0;
			//stateを変える
			m_state = FishState::Walk;
			m_vel.x = m_isRight * kSpeed;
			m_vel.y -= 15.0f;
		}
	}
		break;
	case FishState::Walk:
	{
		Character::BossUpdate();
		//赤
		if (m_num == 1)Walk1();
		if (m_num == 2)Walk2();
		if (m_num == 3)Walk3();
		if (m_num == 4)
		{
			bossTimer--;
			if (bossTimer < 0)
			{
				int rate = GetRand(99);
				int f = 34;
				int s = 69;
				int t = 100;
				if (rate < f)
				{
					selectUpdate = 0;
				}
				if (rate >= f && rate < s)
				{
					selectUpdate = 1;
				}
				if (rate >= s && rate < t)
				{
					selectUpdate = 2;
				}
				bossTimer = 200.0f;
			}
			switch (selectUpdate)
			{
			case 0:
				Walk1();
				if (m_num == 4)
				{

					m_angle += m_isRight ? 2.0f : -2.0f;
				}
				break;
			case 1:
				Walk2();
				if (m_num == 4)
				{

					m_angle += m_isRight ? 8.0f : -8.0f;
				}
				break;
			case 2:
				Walk3();
				if (m_num == 4)
				{

					m_angle += m_isRight ? 8.0f : -8.0f;
				}
				break;
			default: 
				Walk1();
				break;

			}

		}

	}
		break;
	case FishState::knockback:
		/*if (cool_interval - m_coolDamageTimer >= 10.0f)
		{
			m_state = FishState::Walk;
			m_vel.x = m_isRight ? kSpeed : -kSpeed;
		}*/

		//新しいほう
		Character::BossUpdate();
		if (m_coolDamageTimer < 0)
		{
			m_state = FishState::Walk;
			m_vel.x = m_isRight ? kSpeed : -kSpeed;
		}
		break;
	case FishState::Dead:
	{
		m_colRect.SetCenter(1000, 0, 0, 0);
		//マップとの当たり判定
		Character::FishUpdate();
		m_colRect.SetCenter(1000, 0, 0, 0);
		if (charaIdx <= 3)
		{
			m_vel.y = -15.0f;
			m_angle += 2.0f;
			//回転させてもいい
		}
		else
		{
			m_angle = std::lerp(m_angle, 360.0f, 0.5f);
			m_vel.y += 3.0f;
			m_vel.x = 0;
		}
		/*	if (charaIdx == kDeadNum-1 && m_animFrame > kDeadNum * kDeadDuration - 1)
			{
				m_isDead = true;
			}*/
	}
	
		break;
	case FishState::Defeat:
	{
		m_firstFrame++;
		m_colRect.SetCenter(1000, 0, 0, 0);
		if (m_num == 4)
		{
			float shake = sinf(m_firstFrame * 0.6f) * 2.0f;
			m_pos.x = m_pos.x + shake;
		}
		else
		{
			m_pos.y += 1.5f;
		}
		if (m_firstFrame > InitTime)
		{
			m_firstFrame = 0;
			m_animFrame = 0;
			m_isDead = true;

		}
	}
	
		break;
	case FishState::Release:
	{
		m_firstFrame++;
		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_colRect.SetCenter(1000, 0, 0, 0);
		if (m_animFrame < 45)
		{
			m_pos.y -= 13.0f;
			m_angle += 2.0f;
		}
		else
		{
			m_angle = std::lerp(m_angle, 360, 0.4f);
			m_vel.y += 1.5f;
		}
		if (m_firstFrame > 120)
		{
			m_firstFrame = 0;
			m_animFrame = 0;
			m_isDead = true;

		}
	}
	break;
	case FishState::Fusion:
	{
		m_firstFrame++;
		m_colRect.SetCenter(1000, 0, 0, 0);
		m_pos.y -= 1.5f;
		if (m_firstFrame > InitTime)
		{
			m_firstFrame = 0;
			m_animFrame = 0;
			//stateを変える
			m_state = FishState::Fusion2;
			m_vel.y += 5.0f;
		}
	}
		break;
	case FishState::Fusion2:
	{
		m_firstFrame++;
		m_colRect.SetCenter(1000, 0, 0, 0);
		//マップとの当たり判定
		Character::BossUpdate();
		m_colRect.SetCenter(1000, 0, 0, 0);

		float shake = sinf(m_firstFrame * 0.6f) * 2.0f;
		m_pos.x = m_pos.x + shake;
		if (m_firstFrame > InitTime * 2)
		{
			m_firstFrame = 0;
			m_animFrame = 0;
			m_isDead = true;

		}
	}
	   break;
	}

	
}

void Fish::Draw()
{
}
void Fish::Draw(Camera& camera)
{
	
	//描画開始位Y(画面座標)


	if (m_num == 4)//スペシャルフィッシュのとき
	{
		//描画位置を上げる
		float drawY = 0;
		switch (m_state)
		{
		case FishState::First:
			charaIdx = (m_animFrame / kIdleDuration) % kIdleNum;
			charaIdy = kIdleNumY;
			drawY = kSPHeight / 3.0f;
			//描画関連
			{
				//描画の上とした
				float bossTop = m_pos.y - kSPHeight * kScale * 0.5f - drawY;
				float bossBottom = m_pos.y + kSPHeight * kScale * 0.5f - drawY;
				//
				//描画関連
				if (bossTop > groundY)//地面の中
				{
					return;
				}

				if (bossTop <= groundY && bossBottom >= groundY)//地面に挟まっているとき
				{
					srcY = groundY - bossTop;
					srcY = srcY / kScale;
				}
				else
				{
					srcY = kSPHeight;
				}
			}

			break;
		case FishState::Walk:
			charaIdx = (m_animFrame / kWalkDuration) % kWalkNum;
			charaIdy = kWalkNumY;
			drawY = kSPHeight / 2.0f;
			srcY = kSPHeight;
			break;
		case FishState::Dead:
			/*charaIdx = (m_animFrame / kDeadDuration) % kDeadNum;
			charaIdy = kDeadNumY;*/
			drawY = kSPHeight / 2.0f;
			srcY = kSPHeight;
			break;
		case FishState::Defeat:
		
			drawY = kSPHeight / 2.0f;
			srcY = kSPHeight;
			break;
		case FishState::Release:
		/*	charaIdx = 0;
			charaIdy = 3;*/
			drawY = drawY = kSPHeight / 2.0f;
			srcY = kSPHeight;
			break;
		}

		if (m_coolDamageTimer > 0)
		{
			if ((m_coolDamageTimer / 5) % 2 == 0)SetDrawBright(255, 100, 100);//赤っぽく
		}

		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - drawY,
		kSPWidth * 0, kSPHeight * 0,
		kSPWidth, srcY,//切り取りの幅
		kScale, m_angle * DX_PI / 180.0f,//左が拡大率、右が回転率
		m_handle,
		true, m_isRight ? false : true);
	}
	else//普通のFishのとき
	{
		//描画位置を上げる
		float drawY = 0;
		switch (m_state)
		{
		case FishState::First:
			charaIdx = (m_animFrame / kIdleDuration) % kIdleNum;
			charaIdy = kIdleNumY;
			drawY = kCharaHeight / 3.0f;
			//描画関連
			{
				//描画の上とした
				float bossTop = m_pos.y - kCharaHeight * kScale * 0.5f - drawY;
				float bossBottom = m_pos.y + kCharaHeight * kScale * 0.5f - drawY;
				//
				//描画関連
				if (bossTop > groundY)//地面の中
				{
					return;
				}

				if (bossTop <= groundY && bossBottom >= groundY)//地面に挟まっているとき
				{
					srcY = groundY - bossTop;
					srcY = srcY / kScale;
				}
				else
				{
					srcY = kCharaHeight;
				}
			}

			break;
		case FishState::Walk:
			charaIdx = (m_animFrame / kWalkDuration) % kWalkNum;
			charaIdy = kWalkNumY;
			drawY = kCharaHeight / 5.0f;
			srcY = kCharaHeight;
			break;
		case FishState::Dead:
			charaIdx = (m_animFrame / kDeadDuration) % kDeadNum;
			charaIdy = kDeadNumY;
			drawY = kCharaHeight / 5.0f;
			if (m_animFrame >= kDeadNum * kDeadDuration)
			{
				charaIdx = kDeadNum - 1;
			}

			
			break;
		case FishState::Defeat:
			charaIdx = (m_animFrame / kDeadDuration) % kDeadNum;
			charaIdy = kDeadNumY;
			drawY = kCharaHeight / 5.0f;
			if (m_animFrame >= kDeadNum * kDeadDuration)
			{
				charaIdx = kDeadNum - 1;
			}

			//描画関連
			{
				//描画の上とした
				float bossTop = m_pos.y - kCharaHeight * kScale * 0.5f - drawY;
				float bossBottom = m_pos.y + kCharaHeight * kScale * 0.5f - drawY;
				//
				//描画関連
				if (bossTop > groundY)//地面の中
				{
					return;
				}

				if (bossTop <= groundY && bossBottom >= groundY)//地面に挟まっているとき
				{
					srcY = groundY - bossTop;
					srcY = srcY / kScale;
				}
				else
				{
					srcY = kCharaHeight;
				}
			}
			break;
		case FishState::Release:
			charaIdx = 0;
			charaIdy = 3;
		
			srcY = kCharaHeight;
			break;
		case FishState::Fusion:
			charaIdx = (m_animFrame / kIdleDuration) % kIdleNum;
			charaIdy = kIdleNumY;
			drawY = kCharaHeight / 3.0f;
			//drawY = 0;
			//描画関連
			{
				//描画の上とした
				float bossTop = m_pos.y - kCharaHeight * kScale * 0.5f - drawY;
				float bossBottom = m_pos.y + kCharaHeight * kScale * 0.5f - drawY;
				//
				//描画関連
				if (bossTop > groundY)//地面の中
				{
					return;
				}

				if (bossTop <= groundY && bossBottom >= groundY)//地面に挟まっているとき
				{
					srcY = groundY - bossTop;
					srcY = srcY / kScale;
				}
				else
				{
					srcY = kCharaHeight;
				}
			}
			break;
		case FishState::Fusion2:
			charaIdx = (m_animFrame / kIdleDuration) % kIdleNum;
			charaIdy = kIdleNumY;
			drawY = kCharaHeight / 4.5f;
			break;
		}


		if (m_coolDamageTimer > 0)
		{
			if ((m_coolDamageTimer / 5) % 2 == 0)SetDrawBright(255, 100, 100);//赤っぽく
		}




		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - drawY,
		kCharaWidth * charaIdx, kCharaHeight * charaIdy,
		kCharaWidth, srcY,//切り取りの幅
		kScale, m_angle* DX_PI / 180.0f,//左が拡大率、右が回転率
		m_handle,
		true, m_isRight ? false : true);
	}

	//元に戻す
	SetDrawBright(255, 255, 255);
	

#ifdef _DEBUG
	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
	//線
		//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - kActionDistance + camera.drawOffset.x, 0, m_pos.x + kActionDistance + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);

#endif
}

void Fish::HitFishDamage(int damage,bool dir)
{

	if (m_coolDamageTimer > 0)return;
	//SE再生
	Application::GetInstance().GetSoundManager().PlaySE("hitSE");
  m_hp += -damage;

  m_coolDamageTimer = cool_interval;

  if (m_hp <= 0)
  {
	  m_state = FishState::Defeat;
	  m_animFrame = 0;
  }
  else
  {
	  //被弾したらノックバックさせる
	  m_state = FishState::knockback;
	  m_vel.x = dir ? kSpeed/3.0f : -kSpeed / 3.0f;
	 // m_isRight = !dir;

  }
}

void Fish::Walk1()
{
	//何もしない
}

void Fish::Walk2()
{
	//バックのクールタイム
	m_backCoolTime--;

	//playerの近くに来たら一定の距離戻る
	if (std::abs(m_pPlayer->GetPos().x - m_pos.x) < kActionDistance && !m_isBack)
	{
		//クールタイム切ってたらできる
		if (m_backCoolTime <0)m_isBack = true;
	
	}
	//
	if (m_isBack)
	{


		//ここで汗汗みたいなエフェクトを出す。また、音も出したい

		backFrame++;
		m_vel.x = m_isRight ? -kSpeed/2.0 : kSpeed / 2.0;
		if (m_num == 4)
		{

			m_angle += m_isRight ? 8.0f : -8.0f;
		}


		if (backFrame > backTime)
		{
			backFrame = 0;
			m_isBack = false;
			m_backCoolTime = backCoolTime;
			m_vel.x = m_isRight ? kSpeed : -kSpeed;

		}
	}
	else
	{
	
	}
}

void Fish::Walk3()
{
	//playerの近くに来たらジャンプする
	// バックとジャンプを入れ替え
	//バックのクールタイム
	m_backCoolTime--;

	//playerの近くに来たら一定の距離戻る
	if (std::abs(m_pPlayer->GetPos().x - m_pos.x) < kActionDistance && !m_isBack)
	{
		//クールタイム切ってたらできる
		if (m_backCoolTime < 0)m_isBack = true;

	}
	//
	if (m_isBack)
	{

		backFrame++;
		m_vel.y = -10.0f;



		if (m_num == 4)
		{

			m_angle += m_isRight ? 8.0f : -8.0f;
		}

		if (backFrame > jumpTime)
		{
			backFrame = 0;
			m_isBack = false;
			m_backCoolTime = backCoolTime;
			m_vel.x = m_isRight ? kSpeed : -kSpeed;

		}
	}
	else
	{

	}
	
}
