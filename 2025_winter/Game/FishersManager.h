#pragma once
#include "Vec2.h"
#include <vector>
#include <memory>
#include <functional>

class Bg;
class Camera;
class Fish;

enum class FisherPhase
{
	FirstFish,     // 1体目
	SecondFish,    // 2体目
	ThirdFish,     // 3体目
	Fusion,//合体するとき
	SpecialFish,  // 3体後の別の魚
	End
};


class FishersManager
{
public:
	FishersManager(const Vec2& startPos, int dir);
	virtual ~FishersManager();

	void Update(Bg* bg);
	void Draw(Camera& camera);

	std::vector<std::shared_ptr<Fish>> GetFish() { return m_pFishers;}

	int GetHp() { return m_publicHp; }
	bool GetisDead() { return m_isDead; }

private:
	void SpawnFish(Bg* bg);
private:
	FisherPhase m_phase;
	std::vector<std::shared_ptr<Fish>> m_pFishers;
	int m_dir;//-1=左,+1=右

	int m_fishIndex;
	Vec2 m_startPos;
	
private:
	int m_publicHp;
	//spawnするかどうか
	bool m_isSpawn;
	bool m_triSpawn;

	bool m_isfusion;
	Vec2 ReleasePos;
	bool m_isDead = false;
public:
	//着地イベント
	std::vector<std::function<void()>> onLandEvents;
	//リスナー追加用(使いやすくするため)
	void AddOnLandEvent(const std::function<void()>& eventFunc) {
		onLandEvents.push_back(eventFunc);
	}

};

