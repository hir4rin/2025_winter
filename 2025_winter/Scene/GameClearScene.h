#pragma once
#include "Scene.h"
#include "Camera.h"


class Cannon;
class Player;
class Bg;

class GameClearScene :public Scene
{
public:
	GameClearScene(SceneController& controller,PlayerType type);
	void Update(Input& input) override;
	void Draw() override;

	


private:
	/// <summary>
	/// ゴールに到着するための処理(減速処理)
	/// </summary>
	void ToArrivedAtGoal();


	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Cannon> m_pCannon;
	Camera camera;
	Bg* m_pBg;

	bool isCannon = false;//大砲に乗っているかどうか
	bool isFlying = false;//大砲から飛び出しているかどうか

	float m_finalTri = 0.0f;//最終的な三角波の値

	int m_rank = 0;//順位
	float m_gaugeTimer = 0.0f;
	float m_timer = 0.0f;//経過時間
	float m_gaugeleftX = 0.0f;//ゲージの左端
	float m_gaugerightX = 100.0f;//ゲージの右端
	float m_gaugeCursorX = 0.0f;
	//減速用
	float m_slowDownStartX = 0.0f;
	bool isSlowDown = false;//減速中かどうか

};