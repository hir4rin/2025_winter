#pragma once
#include "Vec2.h"
#include "Rect.h"

class Bg;
class Character
{
public:
	Character();
	virtual ~Character();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns>当たり判定情報</returns>
	Rect GetColRect() const { return m_colRect; }
	Vec2 GetPos()const { return m_pos; }

	void Gravity();
	/// <summary>
	/// 当たり判定をセットする
	/// </summary>
	void SetRect();

	void SetBgPointer(Bg* pBg) { m_pBg = pBg; }
protected:
	int m_handle;
	

	int m_x;
	int m_y;
	int m_jumpFrame;//ジャンプフレーム数

	bool m_isRight;//キャラクターが右を向いているかどうか
	bool m_isGround;//キャラクターが地面を向いているかどうか
	bool m_isJumpPreparing;//ジャンプ準備中か

	Vec2 m_pos;
	Vec2 m_vel;
	 Vec2 zero;

	 Rect m_colRect;//当たり判定用の矩形

	 //背景のポインタ(その分ずらすため)
	 Bg* m_pBg;
	
};

