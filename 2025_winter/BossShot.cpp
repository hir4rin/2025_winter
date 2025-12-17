#include "BossShot.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	const float kcharaWidth = 32.0f;//charaの幅
	const float kcharaHeight = 32.0f;//charaの高さ
}

BossShot::BossShot(bool isRight,Vec2 pos) :
	isAlive(true),
	m_hitPlayer(nullptr),
	m_dir(1, 0),
	m_enemydir(isRight)
{
	m_handle = LoadGraph("data/Game/DEAD.png");
	assert(m_handle >= 0);
	Vec2 offset = { 100,0 };
	if (m_enemydir)
	{
		m_pos = pos + offset;
	}
	else
	{
		m_pos = pos - offset;
	}
	

}

BossShot::~BossShot()
{
	DeleteGraph(m_handle);
}

void BossShot::Init()
{

}
void BossShot::Update()
{
	//当たり判定をセット
	m_colRect.SetCenter(m_pos.x, m_pos.y, kcharaWidth, kcharaHeight);



	Vec2 shotVel = m_dir * (m_enemydir ? m_speed : -m_speed);
	m_pos += shotVel;
	//カメラとつないで消す処理を行う必要がある
	if (m_pos.x > AliveLimitX || m_pos.x < 0)
	{
		isAlive = false;
		return;
	}
	//プレイヤーと当たったら消す処理は別

}
void BossShot::Draw()
{

}
void BossShot::CheckPlayer(std::shared_ptr<Player> m_pPlayer)
{
	if (!m_pPlayer) return;
	//ほかの矢に寄せるためこの使用
	//矢が当たっているとき、
	if (m_colRect.IsCollision(m_pPlayer->GetColRect()))
	{
		m_hitPlayer = m_pPlayer;
		isAlive = false;
	}

	//自分と当たった時の処理(未解決)

}
void BossShot::Draw(Camera& camera)
{

	// 弾を描画する
	const float shotHalfW = kCharaSize * 0.5f;
	const float shotHalfH = kCharaSize * 0.5f;
	DrawRotaGraph3(static_cast<int>(m_pos.x) + camera.drawOffset.x,
		static_cast<int>(m_pos.y) + camera.drawOffset.y,
		static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
		kScale, 1.0f,
		0.0f, m_handle,
		true, m_enemydir ? false : true);//もともとm_playerdirだった


	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);

}