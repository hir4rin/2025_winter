#include "Item.h"
#include "DxLib.h"
#include "Camera.h"
#include "EnemyWizard.h"
#include "EnemyArcher.h"

namespace
{
	const Vec2 kInitPos = { 600.0f,100.0f };//初期位置
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr int burning_cut_w = 300;
	constexpr int burning_cut_h = 388;
	constexpr float  player_scale = 3.0f;
	constexpr float  burning_scale = 0.2f;
}




Item::Item(std::shared_ptr<EnemyWizard> _enemywiz)
{
	m_handle = LoadGraph("data/ItemWizard.png");
	m_pos = kInitPos;
	m_state = ItemState::Frozen;
}
Item::Item(std::shared_ptr<EnemyRider> _enemyRiders)
{
	m_handle = LoadGraph("data/BurningCard.png");
	m_pos = kInitPos;
	m_state = ItemState::Burning;
}
Item::Item(std::shared_ptr<EnemyArcher> _enemyArchers)
{
	m_handle = LoadGraph("data/BurningCard.png");
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
}

void Item::Draw()
{
	//当たり判定の描画
	Character::Draw();
	DrawRectRotaGraph(m_pos.x, m_pos.y,
		player_cut_w * 0, player_cut_h * 0,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
}
void Item::Draw(Camera& camera)
{
	//当たり判定の描画
	Character::Draw();
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
		burning_cut_w * 0, burning_cut_h * 0,//切り取り左上
		burning_cut_w, burning_cut_h,//切り取りの幅
		burning_scale, 0.0f, m_handle, true);
	}

	
}
