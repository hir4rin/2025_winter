#pragma once
#include "Character.h"
#include <string>


class Camera;
class Player;

enum class EffectType
{
	Star,
	Dust,
	DustForClear,
	BlueStarLight
};

class Effect : public Character
{
public:
	Effect(Vec2 pos,std::string name);
	virtual ~Effect();

	void Init()override;
	void Update()override;
	void Draw()override;

	void Draw(Camera& camera);

	bool IsDead() { return m_aliveFrame <= 0; }
private:
	int m_animFrame;
	int charaIdx;
	int charaIdy;
	int m_aliveFrame;
	bool m_starDir;

	EffectType m_type;

};

