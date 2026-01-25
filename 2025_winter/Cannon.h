#pragma once
#include <Rect.h>


class Camera;

enum class CannonState
{
	Idle,
	Transform,
	Fire
};

class Cannon
{
public:
	Cannon();
	virtual ~Cannon();
	void Init();
	void Update();
	void Draw(Camera& camera);
	void Fire();

	Rect& GetColRect() { return m_colRect; }
	Vec2& GetPos() { return m_pos; }
	/// <summary>
	/// 大砲が変形するアニメーション
	/// </summary>
	void CannonTransForm() { m_state = CannonState::Transform; m_animFrame = 0; }
	/// <summary>
	/// 大砲が発射するアニメーション
	/// </summary>
	void CannonFire() { m_state = CannonState::Fire; m_animFrame = 0; }
	void TakeCannon();//大砲に乗ったときにでかくなる処理
	void FireCannon();//大砲が発射するときに小さくなる処理

private:
	Rect m_colRect;
	int m_handle;
	int m_animFrame;
	int charaIdx = 0;
	int charaIdy = 0;
	//大砲のサイズ
	float m_Scale = 0.3f;
	int m_count = 0;//大砲のサイズ変更用
	float t = 0;
	float s = 0;

	Vec2 m_pos;
	CannonState m_state = CannonState::Idle;


};

