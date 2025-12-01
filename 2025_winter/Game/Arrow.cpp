#include "Arrow.h"
#include "DxLib.h"
#include "Rect.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	const float kArrowWidth = 32.0f;//Arrowの幅
	const float kArrowHeight = 32.0f;//Arrowの高さ
}

Arrow::Arrow():
	isAlive(true),
	m_dir(1,0),
	m_playerdir(false)
	
{
	m_handle = LoadGraph("data/Game/Arrow.png");
	assert(m_handle >= 0);


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
	//当たり判定をセット
	m_colRect.SetCenter(m_pos.x, m_pos.y, kArrowWidth, kArrowHeight);


	
		Vec2 shotVel = m_dir * (m_playerdir ? m_speed : -m_speed);
		m_pos += shotVel;
		//カメラとつないで消す処理を行う必要がある
		if (m_pos.x > AliveLimitX || m_pos.x < 0)
		{
			isAlive = false;
			return;
		}
	//敵と当たったら消す処理は別
	

}
void Arrow::CheckEnemys(std::vector<std::shared_ptr<EnemyWizard>> _enemyWiz)
{
		for (auto& num : _enemyWiz)//敵と当たったら消す処理
		{
			if (!num) continue;
			bool isHit = m_colRect.IsCollision(num->GetColRect());
			if (isHit)
			{
				//当たった敵を消す
				hitEnemyWizard = num;
				//矢を消す
				isAlive = false;
				//このとき、矢をけす方法を考える
				break;
			}
		}
}
void Arrow::CheckEnemys(std::vector<std::shared_ptr<EnemyRider>> _enemyRiders)
{
		for (auto& num : _enemyRiders)//敵と当たったら消す処理
		{
			if (!num) continue;
			bool isHit = m_colRect.IsCollision(num->GetColRect());
			if (isHit)
			{
				//当たった敵を消す
				hitEnemyRider = num;
				//矢を消す
				isAlive = false;
				//このとき、矢をけす方法を考える
				break;
			}
		}
}
void Arrow::CheckEnemys(std::vector<std::shared_ptr<EnemyArcher>> _enemyArchers)
{
		for (auto& num : _enemyArchers)//敵と当たったら消す処理
		{
			if (!num) continue;
			bool isHit = m_colRect.IsCollision(num->GetColRect());
			if (isHit)
			{
				//当たった敵を消す
				hitEnemyArcher = num;
				//矢を消す
				isAlive = false;
				//このとき、矢をけす方法を考える
				break;
			}
		}
}
void Arrow::Draw()
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
void Arrow::Draw(Camera& camera)
{
	
	
		// 弾を描画する
		const float shotHalfW = kCharaSize * 0.5f;
		const float shotHalfH = kCharaSize * 0.5f;
		DrawRotaGraph3(static_cast<int>(m_pos.x)+ camera.drawOffset.x,
			static_cast<int>(m_pos.y) + camera.drawOffset.y,
			static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
			kScale, 1.0f,
			0.0f, m_handle,
			true, m_playerdir ? false : true);
	

	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);


}

