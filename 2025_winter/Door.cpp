#include "Door.h"
#include "DxLib.h"
#include "Camera.h"

namespace
{
	constexpr int kCharaWidth = 16;
	constexpr int kCharaHeight = 32;
	constexpr float kDoorScale = 5.0f;
}

Door::Door()
{
	m_pos = { 5200,625 };
	m_handle = LoadGraph("data/doors.png");
	charaIdx = 0;
	charaIdy = 0;
}

Door::~Door()
{
	DeleteGraph(m_handle);
}

void Door::Init()
{

}
void Door::Update()
{
	m_colRect.SetCenter(m_pos.x,m_pos.y,kCharaWidth* kDoorScale,kCharaHeight* kDoorScale);
}
void Door::Draw(Camera& camera)
{
	

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y,
		kCharaWidth * charaIdx, kCharaHeight * charaIdy,//切り取り左上
		kCharaWidth, kCharaHeight,//切り取りの幅
		kDoorScale, 0.0f, m_handle, true, false);
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
}
