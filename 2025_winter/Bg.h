#pragma once
#include "Vec2.h"
#include <memory>
class Player;
class Rect;
class Camera;
class Bg
{
public:
	Bg();
	virtual ~Bg();
	/// <summary>
	/// 引き数2つ目はStageNumです
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <param name="stagenum">stage番号のint型を入れる</param>
	Bg(std::shared_ptr<Player> pPlayer,int stagenum);
	
	void Init();
	void Update();
	void Draw(Camera& camera);

	void DrawBg(Camera& camera);
	/// <summary>
	/// マップの読み込み
	/// </summary>
	void LoadMapData();
	/// <summary>
	/// マップの読み込み(stage0)
	/// </summary>
	void LoadMapData0();
	/// <summary>
	/// マップの読み込み(stage1_2)
	/// </summary>
	void LoadMapData1_2();
	/// <summary>
	/// マップの読み込み(stage1_3)
	/// </summary>
	void LoadMapData1_3();
	/// <summary>
	/// マップチップ表示
	/// </summary>
	void DrawMapChip0(Camera& camera);
	/// <summary>
	/// マップチップ表示(1_2)
	/// </summary>
	void DrawMapChip1_2(Camera& camera);
	/// <summary>
	/// マップチップ表示(1_3)
	/// </summary>
	void DrawMapChip1_3(Camera& camera);
	/// <summary>
	/// マップチップ表示
	/// </summary>
	void DrawMapChip(Camera& camera);
	/// <summary>
	/// 指定した矩形と当たっているか判定する
	/// </summary>
	/// <param name="rect">指定した矩形</param>
	/// <param name="chipRect">マップチップの矩形</param>
	/// <returns>当たっているか</returns>
	bool IsCollision(Rect rect, Rect& chipRect);
	bool IsCollision1_1(Rect rect, Rect& chipRect);
	bool IsCollision1_2(Rect rect, Rect& chipRect);
	bool IsCollision1_3(Rect rect, Rect& chipRect);
	/// <summary>
	/// 指定した矩形と当たっているか判定するプレイヤー専用
	/// </summary>
	/// <param name="rect">指定した矩形</param>
	/// <param name="chipRect">マップチップの矩形</param>
	/// <returns>当たっているか</returns>
	bool IsCollisionPlayer(Rect rect, Rect& chipRect);
	bool IsCollisionPlayer1_1(Rect rect, Rect& chipRect);
	bool IsCollisionPlayer1_2(Rect rect, Rect& chipRect);
	bool IsCollisionPlayer1_3(Rect rect, Rect& chipRect);

	
	
private:
	int m_bgH;
	int m_mapH;
	Vec2 m_pos;
	std::shared_ptr<Player>  m_pPlayer;
	int m_mapData;//マップデータ

	int m_chipData[60][17];//チップデータ
	int m_chipData0[300][17];//チップデータ

	// 画像に含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;

	 int StageNum = 1;//stage選択用---//0:stage0 1:最初のマップ

	struct Size
	{
		int width;
		int height;
	};

};

