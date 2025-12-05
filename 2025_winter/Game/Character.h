#pragma once
#include "Vec2.h"
#include "Rect.h"
#include <memory>

class Bg;

struct HitDir
{
	bool any = false;
	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;
};

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
	void  AddPos(Vec2 add) { m_pos += add;}
	Vec2& ChangePos() { return m_pos; }
	void AddVel(Vec2 add) { m_vel += add; }
	Vec2& ChangeVel() { return m_vel; }

	void Gravity();
	/// <summary>
	/// 当たり判定をセットする
	/// </summary>
	void SetRect();

	void SetBgPointer(Bg* pBg)
	{
		m_pBg = pBg;
	}

	bool Getm_isRight() { return m_isRight; }
	/// <summary>
	/// m_isRightを反転させる
	/// </summary>
	void Changem_isRight() { m_isRight = !m_isRight; }

protected:
	int m_handle;
	

	int m_x;
	int m_y;
	int m_jumpFrame;//ジャンプフレーム数

	bool m_isRight;//キャラクターが右を向いているかどうか
	bool m_isGround;//キャラクターが地面を向いているかどうか
	bool m_wasGround;//前のフレームで地面にいたかどうか
	bool m_isJumpPreparing;//ジャンプ準備中か
	bool hitClear;//下から貫通する床に当たっているか

	Vec2 m_pos;
	Vec2 m_vel;
	 Vec2 zero;

	 Rect m_colRect;//当たり判定用の矩形

	 //背景のポインタ(その分ずらすため)
	 Bg* m_pBg;

	 /// <summary>
	/// マップチップとの当たり判定処理
	/// </summary>
	/// <param name="chipRect">マップチップの当たり判定</param>
	 void CheckHitMap(Rect& chipRect);


	 /// <summary>
	 /// プレイヤーでのマップチップとの当たり判定処理
	 /// </summary>
	 /// <param name="chipRect"></param>
	  HitDir  CheckHitMapPlayer(Rect& chipRect);

	 

	 /// <summary>
	 /// プレイヤーと当たったかどうか
	 /// </summary>
	 /// <param name="chipRect"></param>
	 bool CheckHitMapPlayer_(Rect& chipRect);

	 
	
};

