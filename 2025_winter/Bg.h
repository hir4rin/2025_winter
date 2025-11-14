#pragma once
#include "Vec2.h"
class Player;
class Rect;
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
	void LoadMapData();
	/// <summary>
	/// マップチップ表示
	/// </summary>
	void DrawMapChip();
	/// <summary>
	/// 指定した矩形と当たっているか判定する
	/// </summary>
	/// <param name="rect">指定した矩形</param>
	/// <param name="chipRect">マップチップの矩形</param>
	/// <returns>当たっているか</returns>
	bool IsCollision(Rect rect, Rect& chipRect);
private:
	int m_bgH;
	int m_mapH;
	Vec2 m_pos;
	Player* m_pPlayer;
	int m_mapData;//マップデータ

	int m_chipData[60][17];//チップデータ

	// 画像に含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;

	struct Size
	{
		int width;
		int height;
	};

};

