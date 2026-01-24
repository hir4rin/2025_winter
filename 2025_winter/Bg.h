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
	
	void FadeInBg(Camera& camera);

	void FadeOutBg(Camera& camera);
	void FadeOutBg2(Camera& camera);


	void Init();
	void Update();
	void Draw(Camera& camera);
	/// <summary>
	/// スクロールする描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="handle"></param>
	void BgHSetDraw(Camera& camera, int handle, float rate);

	/// <summary>
	/// 背景を描画するところ
	/// </summary>
	/// <param name="camera"></param>
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
	/// マップの読み込み(stageClear)
	/// </summary>
	void LoadMapDataClear();
	/// <summary>
	/// マップの読み込み(stage2_1)
	/// </summary>
	void LoadMapData2_1();
	/// <summary>
	/// マップの読み込み(stage2_2)
	/// </summary>
	void LoadMapData2_2();
	/// <summary>
	/// マップの読み込み(stage2_3)
	/// </summary>
	void LoadMapData2_3();
	void LoadMapDataSub();
	void LoadMapDataBoss();


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
	/// マップチップ表示(stageClear)
	/// </summary>
	void DrawMapChipClear(Camera& camera);
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
	bool IsCollisionClear(Rect rect, Rect& chipRect);
	/// <summary>
	/// 指定した矩形と当たっているか判定するプレイヤー専用
	/// </summary>
	/// <param name="rect">指定した矩形</param>
	/// <param name="chipRect">マップチップの矩形</param>
	/// <returns>当たっているか</returns>
	bool IsCollisionPlayer(Rect rect, Rect& chipRect, bool& hitClearness);
	bool IsCollisionPlayer1_1(Rect rect, Rect& chipRect,bool& hitClearness);
	bool IsCollisionPlayer1_2(Rect rect, Rect& chipRect,bool& hitClearness);
	bool IsCollisionPlayer1_3(Rect rect, Rect& chipRect,bool& hitClearness);

private:
	int m_bgH2=-1;
	int m_bgH3 = -1;
	int m_bgH4 = -1;
	int m_bgH5 = -1;
	int m_bgH6 = -1;
	int m_bgH7 = -1;
	int m_bgH8 = -1;
private:
	int m_bgH=-1;
	int m_mapH = -1;
	int m_fadeH = -1;
	float m_extRate;
	Vec2 m_pos;
	std::shared_ptr<Player>  m_pPlayer;
	int m_mapData;//マップデータ

	int m_chipData[60][17];//チップデータ
	int m_chipData0[300][17];//チップデータ

	// 画像に含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;

	int StageNum = 1;//stage選択用---//0:stage0 1:最初のマップ 2:1_2  3:1_3      4:クリアシーン
	                                          //6:2_1         7:2_2   8:2_3   

	struct Size
	{
		int width;
		int height;
	};
	int m_cloudFrame = 0;//雲を動かす用

};

