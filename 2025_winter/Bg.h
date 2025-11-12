#pragma once
#include "Vec2.h"
//#include "Rect.h"
class Player;
class Bg
{
public:
	Bg();
	virtual ~Bg();
	Bg(Player* pPlayer);
	/// <summary>
	/// 横のスクロール量を決定する
	/// </summary>
	/// <returns></returns>
	int GetScrollX();
	/// <summary>
	/// 縦のスクロール量を決定する
	/// </summary>
	/// <returns></returns>
	int GetScrollY();
	
	void Init();
	void Update();
	void Draw();

	void DrawBg();
private:
	int m_bgH;
	Vec2 m_pos;
	Player* m_pPlayer;
	struct Size
	{
		int width;
		int height;
	};

};

