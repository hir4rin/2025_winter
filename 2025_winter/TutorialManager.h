#pragma once
#include "Vec2.h"
#include <vector>
#include <memory>


class Bg;
class Camera;
class Player;
class Sign;
class ButtonUI;
class EffekseerResourceManager;
enum class TutorialPhase
{
	Move,
	Up,
	Attack,
	Copy,
	CopyOut,
	Jump,
	Dash
};

class TutorialManager
{
public:
	TutorialManager(Vec2 pos, TutorialPhase,std::shared_ptr<EffekseerResourceManager> effRes);
	virtual ~TutorialManager();
	void Update();
	void Draw(Camera& camera);
	std::shared_ptr < Player>& GetTutorialPlayer() { return m_pPlayer; }
private:
	TutorialPhase m_phase;
	//std::vector<std::shared_ptr<Player>> m_pPlayers;
	//std::vector < std::shared_ptr<Sign>> m_pSigns;

	std::shared_ptr<EffekseerResourceManager> m_effRes;

	std::shared_ptr < Player> m_pPlayer;
	std::shared_ptr<Sign> m_pSign;
	std::shared_ptr<ButtonUI> m_pButton;

	int m_frame;
	Vec2 m_offset;
	Vec2 m_startPos;

	//距離を持たせるならば、こいつじたいにm_posをもたせて、距離を図る
};

