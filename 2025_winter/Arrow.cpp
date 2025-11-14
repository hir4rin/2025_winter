#include "Arrow.h"
#include "DxLib.h"
#include "Rect.h"
#include "EnemyWizard.h"
#include "Player.h"


Arrow::Arrow():
	isAlive(false),
	m_dir(1,0),
	m_pPlayer(nullptr),
	m_playerdir(false)
	
{
	m_handle = LoadGraph("data/Arrow.png");

}

Arrow::~Arrow()
{
	DeleteGraph(m_handle);
}
void Arrow::Init()
{

}
void Arrow::Update()
{
	if (isAlive)
	{
		Vec2 shotVel = m_dir * (m_playerdir ? m_speed : -m_speed);
		m_pos += shotVel;

		if (m_pos.x > AliveLimitX || m_pos.x < 0)
		{
			isAlive = false;
			return;
		}
		for (auto& num : _enemyWizard)
		{
			bool isHit = m_colRect.IsCollision(num->GetColRect());
			if (isHit)
			{
				//当たった敵を消す
				hitEnemy = num;
				//矢を消す
				isAlive = false;
				break;
			}
		}
	}	

}
void Arrow::Draw()
{
	//まだDrawScrollのやつしてない
	Character::SetRect();
	if (isAlive == true)
	{
		// 弾を描画する
		const float shotHalfW = kCharaSize * 0.5f;
		const float shotHalfH = kCharaSize * 0.5f;
		DrawRotaGraph3(static_cast<int>(m_pos.x),
			static_cast<int>(m_pos.y),
			static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
			kScale, 1.0f,
			0.0f, m_handle,
			true, m_playerdir ? false : true);
	}


}

void Arrow::SetEnemyWizard(std::vector<EnemyWizard*>& wizards)
{

	_enemyWizard = wizards;
}
