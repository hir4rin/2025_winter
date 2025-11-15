#pragma once
#include "Vec2.h"

/// <summary>
/// 矩形を管理するクラス
/// </summary>
class Rect
{
public:
	Rect();
	~Rect();
	/// <summary>
	/// 矩形の描画
	/// </summary>
	/// <param name="Color"></param>
	/// <param name="isFill"></param>
	void Draw(unsigned int Color, bool isFill);
	/// <summary>
/// スクロール情報を適用した矩形の描画
/// </summary>
/// <param name="scrollX">横スクロール</param>
/// <param name="scrollY">縦スクロール</param>
/// <param name="color">矩形の色</param>
/// <param name="isFill">太さ</param>
	void DrawScroll(int scrollX, int scrollY, unsigned int color, bool isFill);


	void DrawCamera(int cameraX, int cameraY, unsigned int color, bool isFill);
	/// <summary>
	/// 左上座標と幅高さを指定
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetLT(float left, float top, float width, float height);
	/// <summary>
	/// 中心座標と幅高さを指定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetCenter(float x, float y, float width, float height);

	/// <summary>
	/// 矩形の幅を取得
	/// </summary>
	/// <returns></returns>
	float GetWidth() const;

	/// <summary>
	/// 矩形の高さを取得
	/// </summary>
	/// <returns></returns>
	float GetHeight() const;

	/// <summary>
	/// 矩形の中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vec2 GetCenter() const;
	/// <summary>
	/// 矩形の左上のX座標を取得
	/// </summary>
	/// <returns></returns>
	float Getleft() const { return m_left; }
	/// <summary>
	/// 矩形の左上のY座標を取得
	/// </summary>
	/// <returns></returns>
	float GetTop() const { return m_top; }
	/// <summary>
	/// 矩形の右下のX座標を取得
	/// </summary>
	/// <returns></returns>
	float GetRight() const { return m_right; }

	float GetBottom() const { return m_bottom; }

	/// <summary>
	/// 矩形の当たり判定
	/// </summary>
	/// <param name="rect">矩形情報</param>
	/// <returns>当たってるかどうか</returns>
	bool IsCollision(const Rect& rect);

public:
	float m_left; //左上のX座標
	float m_top;//左上のY座標
	float m_right;//右下のX座標
	float m_bottom;//右下のY座標
};

