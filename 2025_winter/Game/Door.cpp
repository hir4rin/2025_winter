#include "Door.h"
#include "DxLib.h"
#include "Camera.h"
#include <cassert>

namespace
{
	constexpr int kCharaWidth = 16;
	constexpr int kCharaHeight = 32;
	constexpr float kDoorScale = 5.0f;
}

Door::Door(Vec2 pos):
	m_frame(0),
	m_pos(pos)
{
	m_handle = LoadGraph("data/doors.png");
	assert(m_handle >= 0);

	charaIdx = 0;
	charaIdy = 0;
}

Door::Door(Vec2 pos,StageID id):
	m_frame(0),
	m_pos(pos)
{
	
	m_handle = LoadGraph("data/doors.png");
	assert(m_handle >= 0);

	charaIdx = 0;
	charaIdy = 0;
	m_id = id;
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

#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
#endif // _DEBUG
}

void Door::OutUpdate()
{
	//fade_interval = 60
	m_frame++;

	charaIdx = m_frame / 15 % 3;
	if (m_frame >= 45) charaIdx = 2;

}
