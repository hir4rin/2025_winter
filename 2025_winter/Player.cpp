#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Pad.h"
#include "Vec2.h"

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置
	constexpr float kSpeed = 20.0f;//移動速度
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



Player::Player():
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
	
	


	AbilityGet();
	
}

void Player::Draw()
{
	
	//Character::Draw();
	

	switch (_anim)
	{
	case  Anim::Idle : 
		 charaIdx = (m_animframe / 6) % 6;
		 charaIdy = 0;
		break;
	case Anim::Walk : 
		 charaIdx = (m_animframe / 8) % 8;
		 charaIdy = 1;
		 break;
	case Anim::Jump :
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
	case Anim::Attack :
		
		charaIdx = (m_animframe / 7) % 7;
			charaIdy = 2;
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
		player_scale, 0.0f, m_handle, true,true);
	}
	
#ifdef _DEBUG
	DrawFormatString(10, 10, GetColor(255, 0, 0), "_animは%dです" ,_anim);
	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです" ,charaIdx);
	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです" ,charaIdy);
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
}

void Player::CopyUpdate()
{
	Character::Gravity();
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

	
}

void Player::AbilityGet()
{
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		//アニメーションの処理を書く
	}
}

void Player::AnimSelect(const Anim&  anim)
{

	if (_anim == Anim::Attack && charaIdx == 6)
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;

	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}
