#include "DxLib.h"
#include "Character.h"
#include "Bg.h"
#include "cassert"

namespace
{
	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ
}


Character::Character() :
	m_handle(-1),
	m_isRight(true),
	m_isGround(false),
	m_wasGround(false),
	m_isJumpPreparing(false),
	m_jumpFrame(0),
	m_x(0),
	m_y(0),
	m_pos(m_x, m_y),
	m_vel(0, 0),
	zero(0, 0),
	hitClear(false)

{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
}

Character::~Character()
{
}

void Character::Init()
{
}

void Character::Update()
{
	//重力処理
	Gravity();
	//敵の重力が弱いから3重にかける
	Gravity();
	Gravity();

	Character::SetRect();

	Rect chipRect;//当たったマップチップの矩形
	CheckHitMap(chipRect);


	if (m_isGround)
	{

		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;


	}

}

void Character::Draw()
{
#ifdef _DEBUG
	//当たり判定の描画//スクロール対応をする
	//m_colRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 255), false);

#endif 
}

void Character::Gravity()
{
	m_vel.y += 1.5f;
}

void Character::SetRect()
{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
}

void Character::CheckHitMap(Rect& chipRect)//imanotokoro________Enemy___dake
{

	//assert(m_pBg && "入っていない");
	// 横から当たったかチェックする
	m_pos.x += m_vel.x;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize-1, kCharaSize-1);

	if (m_pBg->IsCollision(m_colRect, chipRect))
	{
		if (m_vel.x > 0.0f)
		{
			m_pos.x = chipRect.Getleft() - kCharaSize * 0.5f;
		}
		else if (m_vel.x < 0.0f)
		{
			m_pos.x = chipRect.GetRight() + kCharaSize * 0.5f;
		}
		m_vel.x *= -1;
		m_isRight = !m_isRight;
	}

	// 縦から当たったかチェックする
	m_pos.y += m_vel.y;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

	if (m_pBg->IsCollision(m_colRect, chipRect))
	{
		if (m_vel.y > 0.0f)
		{
			m_pos.y = chipRect.GetTop() - kCharaSize * 0.5f;
			m_vel.y = 0.0f;
			m_isGround = true;
		}
		else if (m_vel.y < 0.0f)
		{
			m_pos.y = chipRect.GetBottom() + kCharaSize * 0.5f;
			m_vel.y *= -1.0f;
		}
	}
}

HitDir  Character::CheckHitMapPlayer(Rect& chipRect)
{
	HitDir ans;
	//当たったところを返す

	

	//m_isGroundをセットする
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
	//マップと当たっていなかったらm_isGroundをfalseにする
	if (!(m_pBg->IsCollisionPlayer(m_colRect, chipRect,hitClear))) m_isGround = false;

	

	//assert(m_pBg && "入っていない");
	// 横から当たったかチェックする
	m_pos.x += m_vel.x;


	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

	
	if (m_pBg->IsCollisionPlayer(m_colRect, chipRect,hitClear))
	{

		if (m_vel.x > 0.0f)
		{
			if (hitClear)//透過の床だったら横の判定はしない
			{

			}
			else
			{
				m_pos.x = chipRect.Getleft() - kCharaSize * 0.5f;
				ans.left = true;
			}
			

		}
		else if (m_vel.x < 0.0f)
		{
			m_pos.x = chipRect.GetRight() + kCharaSize * 0.5f;
			ans.right = true;
		}


		m_vel.x = 0.0f;

	}


	// 縦から当たったかチェックする
        m_pos.y += m_vel.y;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

	if (m_pBg->IsCollisionPlayer(m_colRect, chipRect,hitClear))
	{

		if (m_vel.y > 0.0f)
		{
			m_pos.y = chipRect.GetTop() - kCharaSize * 0.5f;
			m_vel.y = 0.0f;
			m_isGround = true;
			ans.bottom = true;
		}
		else if (m_vel.y < 0.0f)
		{
			m_pos.y = chipRect.GetBottom() + kCharaSize * 0.5f;
			m_vel.y *= -1.0f;
			ans.top = true;
		}


	}

	return ans;

}


bool Character::CheckHitMapPlayer_(Rect& chipRect)
{
	m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

	if (m_pBg->IsCollisionPlayer(m_colRect, chipRect,hitClear))
	{


		return true;

	}

	return false;
}
