#pragma once
#include "Character.h"
#include <vector>
#include <memory>


class EnemyWizard;
class EnemyRider;
class Player;
class Camera;
class Arrow :public Character
{

public:
	Arrow();
	virtual ~Arrow();
	void Init()override;
	void Update()override;
	
	void Draw()override;
	void Draw(Camera& camera);

	void CheckEnemys(std::vector<std::shared_ptr<EnemyWizard>> _enemyWiz);
	void CheckEnemys(std::vector<std::shared_ptr<EnemyRider>> _enemyRiders);

	constexpr static int Num = 10;
	Vec2& GetPosition() { return m_pos; }
	
	//void SetEnemyWizard(std::vector<std::shared_ptr<EnemyWizard>>& wizards);//敵のポインタを渡す
	//void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer;}
	
	//ほんとは矢に当たり判定ではなく、SceneMainにそれをやらせたい
	std::shared_ptr<EnemyWizard>  hitEnemyWizard = nullptr;//矢に当たった敵を一時保存
	std::shared_ptr<EnemyRider>  hitEnemyRider = nullptr;//矢に当たった敵を一時保存
public:
	bool isAlive;
	Vec2 m_dir;
	bool m_playerdir;
private:
	const float m_speed = 10;
	float	AliveLimitX = 1920.0f;
	const int kCharaSize = 100;
	
	const float kScale = 3;
	
	

};

