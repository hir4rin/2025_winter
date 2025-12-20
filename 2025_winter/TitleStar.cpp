#include "TitleStar.h"
#include "DxLib.h"

namespace
{
	//star
	constexpr int kStarCutW = 16;
	constexpr int kStarCutH = 16;
	constexpr float kStarScale = 1.0f;
}

TitleStar::TitleStar(Vec2 pos)
{
	m_handle = LoadGraph("data/Game/star.png");
	m_pos = pos;
}

TitleStar::~TitleStar()
{
	DeleteGraph(m_handle);
}

void TitleStar::Update()
{
	m_angle += 0.1f;

}
void TitleStar::Draw()
{
	DrawRectRotaGraph(m_pos.x, m_pos.y,
	kStarCutW * 0, kStarCutH * 0,//切り取り左上
	kStarCutW, kStarCutH,//切り取りの幅
	kStarScale, m_angle * DX_PI / 180.0f, m_handle, true);
}
