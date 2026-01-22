#pragma once
#include "Character.h"

class Camera;

class Sign : public Character
{
public:
	Sign(Vec2 pos,int num);
	Sign(Vec2 pos,int num,int num2);//num2は上との識別用
	virtual ~Sign();


	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

private:
	int m_num;
	int m_progress;
	bool isForm = false;
private:
	int m_fontHandle;// フォントハンドル

};

