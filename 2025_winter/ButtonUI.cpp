#include "ButtonUI.h"
#include "DxLib.h"
#include "Camera.h"


namespace
{
	constexpr int graphWidth = 64;
	constexpr int graphHeight = 64;
	constexpr int kDuration = 25;
	constexpr float kScale = 1.0f;
	constexpr float buttonOffset = 0.0f;
}

ButtonUI::ButtonUI(std::string name,Vec2 pos):
	m_frame(0),
	m_pos(pos)
{

	if (name == "move")
	{
		m_handle = LoadGraph("data/Button/move.png");
		m_handle2 = LoadGraph("data/Button/move2.png");
		drawY = 10;
	}
	if (name == "up")
	{
		m_handle = LoadGraph("data/Button/move.png");
		m_handle2 = LoadGraph("data/Button/up.png");
	}
	if (name == "A")
	{
		m_handle = LoadGraph("data/Button/A.png");
		m_handle2 = LoadGraph("data/Button/A2.png");
	}
	if (name == "X")
	{
		m_handle = LoadGraph("data/Button/X.png");
		m_handle2 = LoadGraph("data/Button/X2.png");
		drawX = -40;
	}
	if (name == "Y")
	{
		m_handle = LoadGraph("data/Button/Y.png");
		m_handle2 = LoadGraph("data/Button/Y2.png");
		drawX = -40;
	}

}

ButtonUI::~ButtonUI()
{
	DeleteGraph(m_handle);
	DeleteGraph(m_handle2);
}
void ButtonUI::Update()
{
	m_frame++;

}
void ButtonUI::Draw(Camera& camera)
{
	if ((m_frame / kDuration % 2) == 0)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x+ drawX, m_pos.y + camera.drawOffset.y - buttonOffset,
		0, 0,
		graphWidth, graphHeight,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle,
		true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x+ drawX, m_pos.y + camera.drawOffset.y- buttonOffset,
		0, 0,
		graphWidth, graphHeight,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle2,
		true);
	}
}
