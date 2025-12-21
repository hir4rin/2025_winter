#include "Sign.h"
#include "Camera.h"
#include "DxLib.h"


namespace
{
	constexpr int kWidth = 48;
	constexpr int kHeight = 48;

	constexpr float kScale = 3.0f;
}

Sign::Sign(Vec2 pos,int num)
{
	m_handle = LoadGraph("data/sign.png");
	m_pos = pos;
	m_num = num;
}

Sign::~Sign()
{
	DeleteGraph(m_handle);
}

void Sign::Init()
{
}

void Sign::Update()
{
}

void Sign::Draw()
{

}
void Sign::Draw(Camera& camera)
{
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		0, 0,
		kWidth, kHeight,//切り取りの幅
		kScale, 0.0f,//左が拡大率、右が回転率
		m_handle,
		true);

	//フォントの大きさを変える
	int oldSize = GetFontSize();
	SetFontSize(32);

	DrawFormatString(m_pos.x + camera.drawOffset.x-20.0f, m_pos.y + camera.drawOffset.y- 30.0f, GetColor(0, 0, 0), "%d位", m_num);

	//フォントサイズを元に戻す(元は16)
	SetFontSize(oldSize);
}
