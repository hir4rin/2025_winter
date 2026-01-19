#pragma once
#include "Character.h"

class Camera;

class Sign : public Character
{
public:
	Sign(Vec2 pos,int num);
	virtual ~Sign();


	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

private:
	int m_num;
private:
	int m_fontHandle;// フォントハンドル

};

