#include "TitleStar.h"
#include "DxLib.h"

namespace
{
	//star
	constexpr int kStarCutW = 16;
	constexpr int kStarCutH = 16;
	constexpr float kStarScale = 3.0f;

	
}

TitleStar::TitleStar(Vec2 pos,float r):
	m_frame(0)
{
	m_handle = LoadGraph("data/Game/star.png");
	m_center = pos;


	m_speed = 0.02f;
	m_angle = 0.0f;
	m_speed = 0.02f;

	m_radius = 0;
	targetRadius = r;
}

TitleStar::~TitleStar()
{
	DeleteGraph(m_handle);
}

void TitleStar::Update()
{
	m_frame++;
	//radiusをだんだん大きくする
	
		m_radius = std::lerp(m_radius, targetRadius, 0.005);



	m_angle += m_speed;

	float angleA = m_angle;
	float angleB = m_angle + DX_PI;//180度反対

	m_posA.x = m_center.x + std::cos(angleA) * m_radius;
	m_posA.y = m_center.y + std::sin(angleA) * m_radius;

	m_posB.x = m_center.x + std::cos(angleB) * m_radius;
	m_posB.y = m_center.y + std::sin(angleB) * m_radius;



}
void TitleStar::Draw()
{
	//回転はいったんなし

	DrawRectRotaGraph(m_posA.x, m_posA.y,
	kStarCutW * 0, kStarCutH * 0,//切り取り左上
	kStarCutW, kStarCutH,//切り取りの幅
	kStarScale, 0.0f * DX_PI / 180.0f, m_handle, true);

	DrawRectRotaGraph(m_posB.x, m_posB.y,
	kStarCutW * 0, kStarCutH * 0,//切り取り左上
	kStarCutW, kStarCutH,//切り取りの幅
	kStarScale, 0.0f * DX_PI / 180.0f, m_handle, true);
}
