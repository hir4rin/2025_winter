#include "EnemyArrow.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>
#include <string>

namespace
{
	const float kColWidth = 32.0f;//Arrowの幅
	const float kColHeight = 32.0f;//Arrowの高さ
	const float kArrowWidth = 100.0f;//Arrowの幅
	const float kArrowHeight = 100.0f;//Arrowの高さ

	const float kScale = 1.0f;

}

EnemyArrow::EnemyArrow():
	isAlive(true),
	m_hitPlayer(nullptr),
	m_dir(1, 0),
	m_enemydir(false),
	m_count(0)
{
	for (int x = 0; x < 30; x++)
	{
		std::string path = "data/Game/Arrow/1/1_" + std::to_string(x) + ".png";
		m_handle[x] = LoadGraph(path.c_str());
	}
	for (int i = 0; i < 30; i++)
	{
		assert(m_handle[i] >= 0);
	}
}

EnemyArrow::~EnemyArrow()
{
	for (int i = 0; i < 30; i++)
	{
		DeleteGraph(m_handle[i]);
	}
}

void EnemyArrow::Init()
{

}
void EnemyArrow::Update()
{
	//当たり判定をセット
	m_colRect.SetCenter(m_pos.x, m_pos.y, kColWidth, kColHeight);



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
void EnemyArrow::Draw(Camera& camera)
{

	m_count = (m_count + 1) % 30;

	// 弾を描画する
	const float shotHalfW = kCharaSize * 0.5f;
	const float shotHalfH = kCharaSize * 0.5f;
	DrawRectRotaGraph(static_cast<int>(m_pos.x) + camera.drawOffset.x,
		static_cast<int>(m_pos.y) + camera.drawOffset.y,
		kArrowWidth * 0, kArrowWidth * 0,
		kArrowWidth, kArrowHeight,
		kScale, 0.0f,
		m_handle[m_count],
		true, m_enemydir ? false : true);

#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif
}
