#include "Item.h"
#include "DxLib.h"
#include "Camera.h"
#include "EnemyWizard.h"
#include "EnemyArcher.h"
#include <cassert>

namespace
{
	const Vec2 kInitPos = { 600.0f,100.0f };//初期位置
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ
	constexpr int kFrozenCutW = 798;
	constexpr int kFrozenCutH = 864;
	//constexpr int kFrozenCutW = 100;
	//constexpr int kFrozenCutH = 100;
	constexpr int burning_cut_w = 992;
	constexpr int burning_cut_h = 1072;
	constexpr int arrow_cut_w = 848;
	constexpr int arrow_cut_h = 1264;
	constexpr float  kFrozenScale = 0.1f;
	constexpr float  burning_scale = 0.1f;
	constexpr float  arrow_scale = 0.07f;


	constexpr float kMaxAngle = 46.0f;
}




Item::Item(std::shared_ptr<EnemyWizard> _enemywiz):
	m_aliveFrame(30),
	m_initAliveFrame(300)
{
	//m_handle = LoadGraph("data/Game/ItemWizard.png");
	m_handle = LoadGraph("data/Game/FrozenCard.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Frozen;
	m_vel.y = -5.0f;
}
Item::Item(std::shared_ptr<EnemyRider> _enemyRiders):
	m_aliveFrame(30),
	m_initAliveFrame(300)
{
	m_handle = LoadGraph("data/Game/BurningCardRE.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Burning;
	m_vel.y = -5.0f;
}
Item::Item(std::shared_ptr<EnemyArcher> _enemyArchers):
	m_aliveFrame(30),
	m_initAliveFrame(300)
{
	m_handle = LoadGraph("data/Game/ArrowCardRE.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Archer;
	m_vel.y = -5.0f;
}

Item::~Item()
{
	DeleteGraph(m_handle);
}

void Item::Init()
{
}

void Item::Update()
{
	m_initAliveFrame--;
	//重力
	m_vel.y += 0.20f;
	Character::FishUpdate();
	//Character::SetRect();
}

void Item::Draw()
{
	
}
void Item::Draw(Camera& camera)
{
#ifdef _DEBUG
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif

	if (m_initAliveFrame < 100)
	{
		if (m_initAliveFrame % 10 == 0)return;
		//SetDrawBright(255, 255, 255, m_initAliveFrame * 4);
	}


	if (m_state == ItemState::Burning)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		burning_cut_w * 0, burning_cut_h * 0,//切り取り左上
		burning_cut_w, burning_cut_h,//切り取りの幅
		burning_scale, 0.0f, m_handle, true);
	}
	else if (m_state == ItemState::Frozen)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		kFrozenCutW * 0, kFrozenCutH * 0,//切り取り左上
		kFrozenCutW, kFrozenCutH,//切り取りの幅
		kFrozenScale, 0.0f, m_handle, true);
	}
	else if (m_state == ItemState::Archer)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		arrow_cut_w * 0, arrow_cut_h * 0,//切り取り左上
		arrow_cut_w, arrow_cut_h,//切り取りの幅
		arrow_scale, 0.0f, m_handle, true);
	}

	
}

void Item::DroppedUpdate()
{
	m_aliveFrame--;
	//effectの動き
	// 重力を受ける
	m_vel.y += 0.25f;
	
	if (m_isRight)
	{
		m_vel.x = -4.0f;
		
		//m_angleをずらす斜めぐらいまで
		  // 徐々に -15° まで傾ける
		m_angle -= 2.0f;
		if (m_angle < -kMaxAngle)
			m_angle = -kMaxAngle;
	}
	else
	{
		m_vel.x = 4.0f;
		// 徐々に +15° まで傾ける
		m_angle += 2.0f;
		if (m_angle > +kMaxAngle)
			m_angle = +kMaxAngle;
	}


	m_pos += m_vel;
}

void Item::DroppedDraw(Camera& camera)
{
	if (m_state == ItemState::Burning)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		burning_cut_w * 0, burning_cut_h * 0,//切り取り左上
		burning_cut_w, burning_cut_h,//切り取りの幅
		burning_scale, m_angle* DX_PI / 180.0f, m_handle, true);
	}
	else if (m_state == ItemState::Frozen)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		kFrozenCutW * 0, kFrozenCutH * 0,//切り取り左上
		kFrozenCutW, kFrozenCutH,//切り取りの幅
		kFrozenScale, m_angle * DX_PI / 180.0f, m_handle, true);
	}
	else if (m_state == ItemState::Archer)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		arrow_cut_w * 0, arrow_cut_h * 0,//切り取り左上
		arrow_cut_w, arrow_cut_h,//切り取りの幅
		arrow_scale, m_angle * DX_PI / 180.0f, m_handle, true);
	}
}
