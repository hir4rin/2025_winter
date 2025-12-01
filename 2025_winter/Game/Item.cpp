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
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr int burning_cut_w = 300;
	constexpr int burning_cut_h = 388;
	constexpr int arrow_cut_w = 268;
	constexpr int arrow_cut_h = 268;
	constexpr float  player_scale = 3.0f;
	constexpr float  burning_scale = 0.2f;
	constexpr float  arrow_scale = 0.2f;
}




Item::Item(std::shared_ptr<EnemyWizard> _enemywiz)
{
	m_handle = LoadGraph("data/Game/ItemWizard.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Frozen;
}
Item::Item(std::shared_ptr<EnemyRider> _enemyRiders)
{
	m_handle = LoadGraph("data/Game/BurningCard.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Burning;
}
Item::Item(std::shared_ptr<EnemyArcher> _enemyArchers)
{
	m_handle = LoadGraph("data/Game/BowAndArrow.png");
	assert(m_handle >= 0);

	m_pos = kInitPos;
	m_state = ItemState::Archer;
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
	Character::Update();
	//Character::SetRect();
}

void Item::Draw()
{
	
}
void Item::Draw(Camera& camera)
{
	//当たり判定の描画
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
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
		player_cut_w * 0, player_cut_h * 0,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
	}
	else if (m_state == ItemState::Archer)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		arrow_cut_w * 0, arrow_cut_h * 0,//切り取り左上
		arrow_cut_w, arrow_cut_h,//切り取りの幅
		arrow_scale, 0.0f, m_handle, true);
	}

	
}
