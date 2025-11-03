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

	constexpr float kJumpPower = 20.0f;//ジャンプ力

	constexpr float kSmallJumpFrame = 8;//小ジャンプのフレーム

	constexpr float kBigJumpHeight = 1.0f;//大ジャンプの高さ

}


Player::Player():
	m_frame(0)
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
	m_frame++;
	Move();
	Character::Update();
}

void Player::Draw()
{
	//Character::Draw();
	int charaIdx = (m_frame / 8) % 5;
	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * charaIdx, player_cut_h * 0,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * charaIdx, player_cut_h * 0,//切り取り左上
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
		jumpHeight = kBigJumpHeight;
	}

}
