#include "Fish.h"
#include "DxLib.h"
#include "Camera.h"
#include "Player.h"

namespace
{
	//スケール
	constexpr float kScale = 2.0f;
	//サイズ
	constexpr int kCharaWidth = 96;
	constexpr int kCharaHeight = 70;
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
	constexpr float kActionDistance = 150.0f;



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


	constexpr int kIdleNum = 14;
	constexpr int kWalkNum = 6;

	constexpr int kIdleNumY = 0;
	constexpr int kWalkNumY = 1;
}

Fish::Fish(Vec2 pos,int num):
	m_animFrame(0),
	m_num(num),
	m_firstFrame(0),
	m_isBack(false),
	m_backCoolTime(0),
	backFrame(0),
	m_angle(0),
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
	if (m_num == 4)
	{

		m_angle += m_isRight ?2.0f : -2.0f;
	}

	switch (m_state)
	{
	case FishState::First:
		m_firstFrame++;
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
		break;
	case FishState::Walk:
		Character::BossUpdate();
		//赤
		if (m_num == 1)Walk1();
		if (m_num == 2)Walk2();
		if (m_num == 3)Walk3();
		if (m_num == 4)Walk1();

		break;

	}

	
}

void Fish::Draw()
{
}
void Fish::Draw(Camera& camera)
{
	
	//描画開始位Y(画面座標)


	if (m_num == 4)
	{
		//描画位置を上げる
		float drawY;
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
		}







		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - drawY,
		kSPWidth * 0, kSPHeight * 0,
		kSPWidth, srcY,//切り取りの幅
		kScale, m_angle * DX_PI / 180.0f,//左が拡大率、右が回転率
		m_handle,
		true, m_isRight ? false : true);
	}
	else
	{
		//描画位置を上げる
		float drawY;
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
			drawY = kCharaHeight / 2.0f;
			srcY = kCharaHeight;
			break;
		}







		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - drawY,
		kCharaWidth * charaIdx, kCharaHeight * charaIdy,
		kCharaWidth, srcY,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle,
		true, m_isRight ? false : true);
	}
	

#ifdef _DEBUG
	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
	//線
		//キャラとプレイヤーとの距離を表示
	DrawBox(m_pos.x - kActionDistance + camera.drawOffset.x, 0, m_pos.x + kActionDistance + camera.drawOffset.x, 1080, GetColor(0, 0, 255), false);

#endif
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
