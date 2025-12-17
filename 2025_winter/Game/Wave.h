#pragma once
#include "Vec2.h"
#include "Rect.h"


class Camera;
/// <summary>
/// 波動単体のクラス
/// </summary>
class Wave
{
public:
	Wave(const Vec2& pos);
	virtual ~Wave();
	void Update();
	void Draw(Camera& camera);

	bool IsDead() { return m_aliveFrame <= 0; }

	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns>当たり判定情報</returns>
	Rect GetColRect() const { return m_colRect; }

private:
	int m_handle = -1;
	int m_aliveFrame;//寿命

	Vec2 m_pos;
	 Rect m_colRect;
};

