#include "Wave.h"
#include "DxLib.h"
#include "Camera.h"
#include "Application.h"
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
	//blueStarLight
	constexpr int kBlueStarLightCutW = 16;
	constexpr int kBlueStarLightCutH = 32;
	constexpr int kBlueStarLightSorW = 30 * 16;//yx
	constexpr int kBlueStarLightSorH = 11 * 16;//yx
	constexpr float kBlueStarLightScale = 4.0f;
	constexpr int kBlueStarLightAnimDuration = 4;
	constexpr int kBlueStarLightAnimNum = 4;
}


Wave::Wave(const Vec2& pos):
	m_pos(pos),
	m_aliveFrame(20),
	m_frame(0)
{
	m_handle = LoadGraph("data/Game/WaterEffect.png");
	assert(m_handle >= 0);
	Application::GetInstance().GetSoundManager().PlaySE("waveSE");
}

Wave::~Wave()
{
	DeleteGraph(m_handle);
}

void Wave::Update()
{
	m_aliveFrame--;
	m_frame++;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kRectSize, kRectSize);
}
void Wave::Draw(Camera& camera)
{
	int charaIdx = 0;
	charaIdx = 2+ (m_frame / kBlueStarLightAnimDuration) % kBlueStarLightAnimNum;

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y - kBlueStarLightCutH* kBlueStarLightScale*1/4,
			 kBlueStarLightSorW + kBlueStarLightCutW * charaIdx, kBlueStarLightSorH,//切り取り座標
			 kBlueStarLightCutW, kBlueStarLightCutH,//切り取りの幅
			 kBlueStarLightScale, 0.0f,//左が拡大率、右が回転率
			 m_handle,
			 true,
			 true,//反転するかどうか
			  false);
#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
#endif
}
