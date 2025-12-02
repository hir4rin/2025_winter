#pragma once

class Camera;
class StageUI
{
public:
	StageUI();
	virtual ~StageUI();

	void Init();
	void Update();
	void Draw(Camera& camera);
};

