#include "EnemyArrow.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include "Camera.h"


namespace
{
	const float kArrowWidth = 32.0f;//Arrowの幅
	const float kArrowHeight = 32.0f;//Arrowの高さ
}

EnemyArrow::EnemyArrow():
	isAlive(true),


	m_dir(1, 0),
	m_enemydir(false)
{
	m_handle = LoadGraph("data/EnemyArrow.png");
}

EnemyArrow::~EnemyArrow()
{
	DeleteGraph(m_handle);
}

void EnemyArrow::Init()
{

}
void EnemyArrow::Update()
{
	//当たり判定をセット
	m_colRect.SetCenter(m_pos.x, m_pos.y, kArrowWidth, kArrowHeight);



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
void EnemyArrow::Draw()
{

}
void EnemyArrow::CheckPlayer(std::shared_ptr<Player> m_pPlayer)
{
	//if (!m_pPlayer) return;

	//自分と当たった時の処理(未解決)

}
void EnemyArrow::Draw(Camera& camera)
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
