#include "Cannon.h"
#include "DxLib.h"
#include "Camera.h"
#include <cassert>

namespace
{
		constexpr int kCannonCutW = 62;
	constexpr int kCannonCutH = 46;
	constexpr float  kCannonScale = 3.0f;
	constexpr int  kCannonDuration = 10;
	constexpr int  kCannonNum = 7;
}


Cannon::Cannon():
	charaIdx(0),
	charaIdy(0),
	m_animFrame(0),
	m_pos(300.0f, 700.0f)
{
	 m_handle = LoadGraph("data/cannon.png");
	assert(m_handle != -1 && "Cannon画像の読み込みに失敗しました");
	m_colRect.SetCenter(0.0f, 0.0f, 64.0f, 64.0f);
}

Cannon::~Cannon()
{
}

void Cannon::Init()
{
}
void Cannon::Update()
{
	m_animFrame++;
	m_colRect.SetCenter(m_pos.x, m_pos.y, 64.0f, 64.0f);

}
void Cannon::Draw(Camera& camera)
{
	charaIdx = (m_animFrame / kCannonDuration) % kCannonNum;
	charaIdy = 0;


	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(255, 255, 255), false);

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		kCannonCutW * charaIdx, kCannonCutH * charaIdy,//切り取り左上
		kCannonCutW, kCannonCutH,//切り取りの幅
		kCannonScale, 0.0f, m_handle, true);

}
void Cannon::Fire()
{
}
