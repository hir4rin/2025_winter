#pragma once
#include "Character.h"

class Camera;
class Salmon :public Character
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">pos</param>
	/// <param name="dir">向き</param>
	/// <param name="num">何番目のサーモン（飛び方が変わる）</param>
	Salmon(Vec2 pos,bool dir,int num);
	virtual ~Salmon();

	void Init()override;
	void Update()override;
	void Draw()override;
	void Draw(Camera& camera);

private:
	int m_angle;
	int m_Num;


};

