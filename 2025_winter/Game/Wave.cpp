#include "Wave.h"
#include "DxLib.h"
#include "Camera.h"
#include <cassert>

namespace
{

	constexpr int enemy_cut_w = 50;
	constexpr int enemy_cut_h = 50;
	constexpr float kScale = 2.0f;

	constexpr int kRectSize = 32;

	//表示させる画像
	constexpr int kCharaIdx = 15;
	//Y座標を少し上げる
	constexpr float drawY = 1.5f / 3.0f;
}


Wave::Wave(const Vec2& pos):
	m_pos(pos),
	m_aliveFrame(20)
{
	m_handle = LoadGraph("data/Game/exp ice.png");
	assert(m_handle >= 0);
}

Wave::~Wave()
{
	DeleteGraph(m_handle);
}

void Wave::Update()
{
	m_aliveFrame--;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kRectSize, kRectSize);
}
void Wave::Draw(Camera& camera)
{

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - enemy_cut_h * drawY,
	enemy_cut_w * kCharaIdx, 0,
	enemy_cut_w, enemy_cut_h,//切り取りの幅
	kScale, 0.0f,//左が拡大率、右が回転率
	m_handle,
	true, false);


#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
#endif
}
