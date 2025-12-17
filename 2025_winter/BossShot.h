#pragma once
#include "Character.h"
#include <vector>
#include <memory>


class Camera;
class Player;

/// <summary>
/// ボスが撃つ弾のクラス
/// </summary>
class BossShot : public Character
{
public:
	BossShot(bool isRight,Vec2 pos);
	virtual ~BossShot();
	void Init()override;
	void Update()override;

	void Draw(Camera& camera);
	void Draw()override;

	void CheckPlayer(std::shared_ptr<Player> m_pPlayer);
	constexpr static int Num = 10;
	Vec2& GetPosition() { return m_pos; }

	std::shared_ptr<Player> m_hitPlayer;


public:
	bool isAlive;
	Vec2 m_dir;
	bool m_enemydir;
private:
	const float m_speed = 10;
	float	AliveLimitX = 5000.0f;
	const int kCharaSize = 32;


	const float kScale = 3;
};

