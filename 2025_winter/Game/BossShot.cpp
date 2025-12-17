#include "BossShot.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	constexpr float kcharaWidth = 50.0f;//charaの幅
	constexpr float kcharaHeight = 50.0f;//charaの高さ

	constexpr float kGraphWidth = 1024.0f;
	constexpr float kGraphHeight = 1024.0f;

	constexpr int kCharaSize = 32;


	constexpr float kScale = 0.125f;
}

BossShot::BossShot(bool isRight,Vec2 pos) :
	isAlive(true),
	m_hitPlayer(nullptr),
	m_dir(1, 0),
	m_enemydir(isRight)
{
	m_handle = LoadGraph("data/Game/bomb.png");
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


	//画像サイズを取得
//画像のサイズ
	int graphW = 0;
	int graphH = 0;

	GetGraphSize(m_handle, &graphW, &graphH);

	// 弾を描画する
	
	DrawRectRotaGraph(static_cast<int>(m_pos.x) + camera.drawOffset.x,//座標
		static_cast<int>(m_pos.y) + camera.drawOffset.y,//座標
		0,0,//切り取り座標
		graphW, graphH,//幅
		kScale,0.0f,
		m_handle,
		true, m_enemydir ? true : false);//もともとm_playerdirだった

#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif
}