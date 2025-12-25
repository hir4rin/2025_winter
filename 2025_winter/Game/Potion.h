#pragma once
#include "Character.h"

class Camera;

class Potion : public Character
{
public:
	Potion(Vec2 pos);
	virtual ~Potion();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Draw(Camera& camera);

private:

};

