#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Pad.h"

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置
	constexpr float kSpeed = 20.0f;//移動速度
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr float  player_scale = 1.0f;

}


Player::Player()
{
	m_pos = kInitPos;
	m_handle = LoadGraph("data/player.png");
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
	Move();
	Character::Update();
}

void Player::Draw()
{
	//Character::Draw();
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * 0, player_cut_h * 0,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * 0, player_cut_h * 0,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true,true);
	}
	
#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(255, 0, 0), "AAAAAAAAAAA");
#endif
}

void Player::Move()
{
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
}
