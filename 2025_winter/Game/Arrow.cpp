#include "Arrow.h"
#include "DxLib.h"
#include "Rect.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "EnemyEliteOrc.h"
#include "EnemyBear.h"
#include "EnemyWolf.h"
#include "Fish.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>
#include <string>

namespace
{
	const float kColWidth = 32.0f;//Arrowの幅
	const float kColHeight = 32.0f;//Arrowの高さ

	const float kArrowWidth = 100.0f;//Arrowの幅
	const float kArrowHeight = 100.0f;//Arrowの高さ

	constexpr float kScale = 1.0f;
	
}

Arrow::Arrow():
	isAlive(true),
	m_dir(1,0),
	m_playerdir(false),
	m_count(0)
	
{
	//m_handle = LoadGraph("data/Game/Arrow.png");
	for (int x = 0; x < 30; x++)
	{
		std::string path = "data/Game/Arrow/1/1_" + std::to_string(x) + ".png";
		m_handle[x] = LoadGraph(path.c_str());
	}
	for (int i = 0; i < 30; i++)
	{
		assert(m_handle[i] >= 0);
	}


}

Arrow::~Arrow()
{
	for (int i = 0; i < 30; i++)
	{
		DeleteGraph(m_handle[i]);
	}
}
void Arrow::Init()
{

}
void Arrow::Update()
{
	//当たり判定をセット
	m_colRect.SetCenter(m_pos.x, m_pos.y, kColWidth, kColHeight);
	


	
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
void Arrow::CheckEnemys(std::shared_ptr<EnemyEliteOrc> _enemyOrc)
{
	//敵と当たったら消す処理
	
	if (!_enemyOrc) return;
		bool isHit = m_colRect.IsCollision(_enemyOrc->GetColRect());
		if (isHit)
		{
			//当たった敵を消す
			hitEnemyEliteOrc = _enemyOrc;
			//矢を消す
			isAlive = false;
			//このとき、矢をけす方法を考える
			
		}
	
}
void Arrow::CheckEnemys(std::shared_ptr<EnemyBear> _enemybear)
{
	//敵と当たったら消す処理
	
	if (!_enemybear) return;
		bool isHit = m_colRect.IsCollision(_enemybear->GetColRect());
		if (isHit)
		{
			//当たった敵を消す
			hitEnemyBear = _enemybear;
			//矢を消す
			isAlive = false;
			//このとき、矢をけす方法を考える
			
		}
	
}
void Arrow::CheckEnemys(std::shared_ptr<EnemyWolf> _enemywolf)
{
	//敵と当たったら消す処理
	
	if (!_enemywolf) return;
		bool isHit = m_colRect.IsCollision(_enemywolf->GetColRect());
		if (isHit)
		{
			//当たった敵を消す
			hitEnemyWolf = _enemywolf;
			//矢を消す
			isAlive = false;
			//このとき、矢をけす方法を考える
			
		}
	
}
void Arrow::CheckEnemys(std::vector<std::shared_ptr<Fish>> _enemyfishers)
{
	if (_enemyfishers.size() == 0) return;

	//敵と当たったら消す処理
	for (auto& num : _enemyfishers)
	{
		if (!num) continue;
		bool isHit = m_colRect.IsCollision(num->GetColRect());
		if (isHit)
		{
			//当たった敵を消す
			hitEnemyFish = num;
			//矢を消す
			isAlive = false;
			//このとき、矢をけす方法を考える
			break;
		}
	}
	
	
}
void Arrow::Draw()
{
	
	
	
		//// 弾を描画する
		//const float shotHalfW = kCharaSize * 0.5f;
		//const float shotHalfH = kCharaSize * 0.5f;
		//DrawRotaGraph3(static_cast<int>(m_pos.x),
		//	static_cast<int>(m_pos.y),
		//	static_cast<int>(shotHalfW), static_cast<int>(shotHalfH),
		//	kScale, 1.0f,
		//	0.0f, m_handle,
		//	true, m_playerdir ? false : true);
	


}
void Arrow::Draw(Camera& camera)
{
	m_count = (m_count + 1) % 30;
	
		// 弾を描画する
		const float shotHalfW = kCharaSize * 0.5f;
		const float shotHalfH = kCharaSize * 0.5f;
		DrawRectRotaGraph(static_cast<int>(m_pos.x)+ camera.drawOffset.x,
			static_cast<int>(m_pos.y) + camera.drawOffset.y,
			kArrowWidth*0, kArrowWidth*0,
			kArrowWidth,kArrowHeight,
			kScale, 0.0f,
			m_handle[m_count],
			true, m_playerdir ? false : true);
	
#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
#endif

}

