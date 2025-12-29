#include "Fish.h"
#include "DxLib.h"
#include "Camera.h"

namespace
{
	constexpr float kScale = 1.0f;
	constexpr int kCharaWidth = 96;
	constexpr int kCharaHeight = 70;


	//
	constexpr int kWalkDuration = 10;


	constexpr int kWalkNum = 6;

	constexpr int kWalkNumY = 1;
}

Fish::Fish(Vec2 pos,int num):
	m_animframe(0),
	m_num(num)
{
	switch (m_num)
	{
	case 1:
		m_handle = LoadGraph("data/Game/Fish1.png");
		break;
	case 2:
		m_handle = LoadGraph("data/Game/Fish1.png");
		break;
	case 3:
		m_handle = LoadGraph("data/Game/Fish1.png");
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
	m_animframe++;
}

void Fish::Draw()
{
}
void Fish::Draw(Camera& camera)
{
	charaIdx = (m_animframe / kWalkDuration) % kWalkNum;

	charaIdy = kWalkNumY;

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	kCharaWidth * charaIdx, kCharaHeight * charaIdy,
	kCharaWidth, kCharaHeight,//切り取りの幅
	kScale, 0.0f,//左が拡大率、右が回転率
	m_handle,
	true, false);

#ifdef _DEBUG
	//判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif
}
