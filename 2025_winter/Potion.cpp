#include "Potion.h"
#include "Camera.h"
#include "DxLib.h"

namespace
{
	constexpr int kColWidth = 32;
	constexpr int kColHeight = 32;

	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;

	constexpr int kScale = 2.0f;

}

Potion::Potion(Vec2 pos)
{
	m_pos = pos;
	m_handle = LoadGraph("data/Game/heart.png");
}

Potion::~Potion()
{
	DeleteGraph(m_handle);
}

void Potion::Init()
{
}

void Potion::Update()
{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kColWidth, kColHeight);
}

void Potion::Draw()
{
}
void Potion::Draw(Camera& camera)
{
#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
#endif

	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	kGraphWidth * 0, kGraphHeight * 0,//切り取り左上
	kGraphWidth, kGraphHeight,//切り取りの幅
	kScale, 0, m_handle, true);

}
