#include "DxLib.h"
#include "Effect.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	constexpr int kStarCutW = 16;
	constexpr int kStarCutH = 16;
	constexpr float kScale = 1.0f;
}

Effect::Effect(std::shared_ptr<Player> _player,std::string name):
	m_aliveFrame(20),
	m_starDir(true)
{
	m_pos = _player->GetPos();
	if (name == "star")
	{
		m_handle = LoadGraph("data/Game/star.png");
		assert(m_handle != -1 && "Failed to load star image");
	}
	//m_vel.y = -10.0f;
	//ランダムなx方向の速度
	float vx = (GetRand(60) - 30) / 10.0f;//-3.0f ~ 3.0f
	float vy = (GetRand(60) - 20) / 10.0f;//-1.0f ~ 4.0f

	m_vel = { vx, vy };
}

Effect::~Effect()
{
	DeleteGraph(m_handle);
}

void Effect::Init()
{

}
void Effect::Update()
{
	m_aliveFrame--;
	//effectの動き
	// 重力を受ける
	m_vel.y += 0.25f;

	

	m_pos += m_vel;
}
void Effect::Draw()
{

}

void Effect::Draw(Camera& camera)
{
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	0, 0,
	kStarCutW, kStarCutH,//切り取りの幅
	kScale, 0.0f,//左が拡大率、右が回転率
	m_handle,
	true,
	m_starDir ? false : true,//反転するかどうか
		false);
}
