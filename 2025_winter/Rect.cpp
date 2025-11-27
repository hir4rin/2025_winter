#include "Rect.h"
#include "DxLib.h"
#include <cassert>

Rect::Rect() :
	m_left(0.0f),
	m_top(0.0f),
	m_right(0.0f),
	m_bottom(0.0f)
{

}
Rect::~Rect()
{

}

void Rect::Draw(unsigned int color, bool isFill)
{
	DrawBox(static_cast<int>(m_left), static_cast<int>(m_top), static_cast<int>(m_right), static_cast<int>(m_bottom),color,isFill);
}


void Rect::SetLT(float left, float top, float width, float height)
{
	m_left = left;
	m_top = top;
	m_right = left + width;
	m_bottom = top + height;
}
void Rect::SetCenter(float x, float y, float width, float height)
{
	//ここに処理を書く
	m_left = x - width / 2;
	m_top = y - height / 2;
	m_right = x + width / 2;
	m_bottom = y + height / 2;
}

float Rect::GetWidth() const
{
	assert(m_right >= m_left);//左右の座標入れ替わりチェック
	return m_right - m_left;
}

//cosntはそのクラスで使われている変数は変わらないということ
//assertは止まるやつ(満たしていなかったら)
float Rect::GetHeight() const
{
	assert(m_bottom >= m_top);//上下の入れ替わりチェック
	return m_bottom - m_top;
}
Vec2 Rect::GetCenter()const
{
	//中心座標
	float x = (m_left + m_right) / 2;
	float y = (m_top + m_bottom) / 2;

	return Vec2{ x,y };
}
bool Rect::IsCollision(const Rect& rect)
{
	//絶対に当たらないパターンをはじいていく
	if (m_left > rect.m_right) return false;
	if (m_top > rect.m_bottom) return false;
	if (m_right < rect.m_left) return false;
	if (m_bottom < rect.m_top) return false;
	 //当たらないパターン以外は当たっている
	return true;
}

bool Rect::CheckLeftHit(const Rect& rect)
{
	//ぶつかっている前提の時、
	//左からぶつかっているか
	if (m_left < rect.Getleft())
	{
		return true;
	}
	return false;
}

void Rect::DrawScroll(int scrollX, int scrollY, unsigned int color, bool isFill)
{
	int drawL = static_cast<int>(m_left) - scrollX;
	int drawT = static_cast<int>(m_top) - scrollY;
	int drawR = static_cast<int>(m_right) - scrollX;
	int drawB = static_cast<int>(m_bottom) - scrollY;
	DrawBox(drawL, drawT, drawR, drawB, color, isFill);
}

void Rect::DrawCamera(int cameraX, int cameraY, unsigned int color, bool isFill)
{
	int drawL = static_cast<int>(m_left) + cameraX;
	int drawT = static_cast<int>(m_top) + cameraY;
	int drawR = static_cast<int>(m_right) + cameraX;
	int drawB = static_cast<int>(m_bottom) + cameraY;
	DrawBox(drawL, drawT, drawR, drawB, color, isFill);
}
