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

private:
	Rect m_colRect;
	int m_handle;
	int m_animFrame;
	int charaIdx = 0;
	int charaIdy = 0;

	Vec2 m_pos;
	CannonState m_state = CannonState::Idle;


};

