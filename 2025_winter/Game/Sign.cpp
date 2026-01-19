#include "Sign.h"
#include "Camera.h"
#include "DxLib.h"
#include <string>


namespace
{
	constexpr int kWidth = 48;
	constexpr int kHeight = 48;

	constexpr float kScale = 3.0f;
}

Sign::Sign(Vec2 pos,int num):
	m_fontHandle(-1)
{
	m_handle = LoadGraph("data/sign.png");
	m_pos = pos;
	m_num = num;
	//フォントの生成
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 48, 6, -1);
}

Sign::~Sign()
{
	DeleteGraph(m_handle);
	//生成したフォントの削除
	DeleteFontToHandle(m_fontHandle);
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

	//DrawFormatString(m_pos.x + camera.drawOffset.x-20.0f, m_pos.y + camera.drawOffset.y- 30.0f, GetColor(0, 0, 0), "%d", m_num);
	std::string i = std::to_string(m_num);

	DrawStringToHandle(m_pos.x + camera.drawOffset.x - 15.0f, m_pos.y + camera.drawOffset.y - 40.0f, i.c_str(), GetColor(0, 0, 0), m_fontHandle);

	//フォントサイズを元に戻す(元は16)
	SetFontSize(oldSize);
}
