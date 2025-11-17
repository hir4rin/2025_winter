#pragma once
#include "Character.h"

class Camera;

class Item :public Character
{
public:
	Item();
	virtual ~Item();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Draw(Camera& camera);

};

