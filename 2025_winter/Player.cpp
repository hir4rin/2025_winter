#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Pad.h"
#include "Vec2.h"
#include <cassert> // ←assert用

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置
	constexpr float kSpeed = 20.0f;//移動速度
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr float  player_scale = 3.0f;

	constexpr float kJumpPower = 20.0f;//ジャンプ力

	constexpr float kSmallJumpFrame = 5;//小ジャンプのフレーム


	constexpr float kSmallJumpHeight = 1.0f;//小ジャンプの高さ

	constexpr float kBigJumpHeight = 1.5f;//大ジャンプの高さ

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置

}



Player::Player() :
	m_frame(0),
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isNomove(false)
{
	m_pos = kInitPos;
	m_handle = LoadGraph("data/player.png");
	_anim = Anim::Idle;
	_state = PlayerState::Normal;
}
Player::~Player()
{
	DeleteGraph(m_handle);
}

void Player::Init()
{
}

void Player::Update()
{
	m_frame++;
	m_animframe++;

	//当たり判定更新
	Character::SetRect();

	AnimSelect(_anim);

	InputUpdate();//特殊行動の入力検知
	switch (_state)
	{
	case PlayerState::Normal:
		NormalUpdate();
		break;
	case PlayerState::Attack:
		AttackUpdate();
		break;
	case PlayerState::Copy:
		CopyUpdate();
		break;
	}






}

void Player::Draw()
{
#ifdef _DEBUG
	//当たり判定の描画
	Character::Draw();
	m_attackRect.Draw(GetColor(0, 255, 0), false);
#endif

	switch (_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / 6) % 6;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / 8) % 8;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 5)
		{
			charaIdx = 5;
			charaIdy = 3;
		}
		else
		{
			charaIdx = (m_animframe / 10) % 2 + 4;
			charaIdy = 3;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / 7) % 7;
		charaIdy = 2;
		break;
	case Anim::Copy:
		charaIdx = (m_animframe / 10) % 6;
		charaIdy = 4;
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true, true);
	}

#ifdef _DEBUG
	DrawFormatString(10, 10, GetColor(255, 0, 0), "_animは%dです", _anim);
	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです", charaIdx);
	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです", charaIdy);
#endif
}

void Player::InputUpdate()
{
	//特殊行動の入力検知
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		_state = PlayerState::Attack;
	}
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		_state = PlayerState::Copy;
	}
}

void Player::NormalUpdate()
{
	Move();
	Jump();
	Character::Gravity();
	m_pos += m_vel;
	//着地時にアニメーションを帰るところ
	if (m_pos.y > kGround)
	{
		m_pos.y = kGround;
		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		if (_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}
}

void Player::JumpUpdate()
{
	Jump();
	Character::Gravity();
	m_pos += m_vel;

}

void Player::AttackUpdate()
{

	Character::Gravity();
	m_pos += m_vel;
	Attack();
	//着地時にアニメーションを帰るところ
	if (m_pos.y > kGround)
	{
		m_pos.y = kGround;
		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		if (_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}
	//攻撃判定が当たったときの処理を書く
}

void Player::CopyUpdate()
{
	Character::Gravity();
	Copy();
	m_pos += m_vel;
}

void Player::Move()
{
	AnimSelect(_anim);
	if (isNomove)return;
	//ここに処理を追加していく
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_vel.x = -kSpeed;
		m_isRight = false;


	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_vel.x = kSpeed;
		m_isRight = true;

	}
	else
	{
		m_vel.x = 0.0f;

	}
	if (_anim != Anim::Jump)
	{
		if (m_vel.x > 0.1f || m_vel.x < -0.1f)
		{
			AnimSelect(Anim::Walk);
		}
		else
		{
			AnimSelect(Anim::Idle);
		}
	}
}
void Player::Jump()
{
	//ジャンプ中はスキップ
	if (!m_isGround) return;

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isJumpPreparing = true;

	}

	if (!m_isJumpPreparing) return;
	m_jumpFrame++;

	float jumpHeight = kJumpPower;//ジャンプの高さを決める

	//ボタンを離した瞬間にジャンプする
	if (!Pad::IsRelease(PAD_INPUT_1)) return;

	if (m_jumpFrame < kSmallJumpFrame)
	{
		jumpHeight = kSmallJumpHeight;
	}
	else
	{
		jumpHeight = kBigJumpHeight;
	}

	m_vel.y = -kJumpPower * jumpHeight;
	m_isGround = false;
	m_isJumpPreparing = false;
	AnimSelect(Anim::Jump);

}

void Player::Attack()
{

	isNomove = true;
	m_vel = zero;
	//アニメーション
	AnimSelect(Anim::Attack);
	//判定をつける

	m_attackRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -70.0f), m_pos.y - kCharaSize, 60.0f, 80.0f);


}

void Player::Copy()
{
	isNomove = true;
	m_vel = zero;
	//アニメーション
	AnimSelect(Anim::Copy);
	//判定をつける
}

void Player::AnimSelect(const Anim& anim)
{

	if (_anim == Anim::Attack && charaIdx == 6)
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}
	if (_anim == Anim::Copy && charaIdx == 5)
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;
	if (_anim == Anim::Copy)return;


	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}
