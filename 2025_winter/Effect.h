#pragma once
#include "Character.h"
#include <string>


class Camera;
class Player;

class Effect : public Character
{
public:
	Effect(std::shared_ptr <Player> _player,std::string name);
	virtual ~Effect();

	void Init()override;
	void Update()override;
	void Draw()override;

	void Draw(Camera& camera);

	bool IsDead() { return m_aliveFrame <= 0; }
private:
	int m_aliveFrame;
	bool m_starDir;

};

