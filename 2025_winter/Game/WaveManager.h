#pragma once
#include "Vec2.h"
#include <vector>
#include <memory>

class Bg;
class Camera;
class Wave;

class WaveManager
{
public:
	WaveManager(const Vec2& startPos, int dir);
	virtual  ~WaveManager();

	void Update(Bg* bg);
	void Draw(Camera& camera);

	bool IsFinished()const { return m_finished; }

	std::vector<std::unique_ptr<Wave>>& GetWaves() { return m_waves; }

private:
	void SpawnWave(Bg* bg);
private:
	std::vector<std::unique_ptr<Wave>> m_waves;

	Vec2 m_startPos;//波動開始位置
	int m_dir;//-1=左,+1=右
	int m_step = 0;//何マス目か

	int m_spawnTimer = 0;
	int m_spawnInterval = 6;//6フレーム事

	int m_maxStep = 10;//最大10マス
	bool m_finished = false;




};

