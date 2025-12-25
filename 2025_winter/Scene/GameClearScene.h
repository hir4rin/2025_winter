#pragma once
#include "Scene.h"
#include "Camera.h"
#include "StageUI.h"


class Cannon;
class Sign;
class Player;
class Bg;
class Effect;

class GameClearScene :public Scene
{
public:
	GameClearScene(SceneController& controller,PlayerType type,int hp,int Life);
	void Update(Input& input) override;
	void Draw() override;

	

private:
	//エフェクト
	std::vector<std::shared_ptr<Effect>> m_pEffects;

private:
	int m_frame = 0;	// フェードインアウト用
	//-----------Updateメンバ関数ポインタ---------
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(GameClearScene::*)(Input&);
	UpdateFunc_t update_;	// Update系を受け取るメンバ関数ポインタ
	//-------------------------------------------------------------
	//-----------------------Drawメンバ関数ポインタ------------------------

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(GameClearScene::*)();
	DrawFunc_t draw_;	// Draw系を受け取るメンバ関数ポインタ
	//------------------------------------------------------------
	/// <summary>
	/// ゴールに到着するための処理(減速処理)
	/// </summary>
	void ToArrivedAtGoal();


	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Cannon> m_pCannon;
	std::vector<std::shared_ptr<Sign>> m_pSigns;
	Camera camera;
	Bg* m_pBg;
	StageUI stageUI;//UI

	bool isCannon = false;//大砲に乗っているかどうか
	bool isFlying = false;//大砲から飛び出しているかどうか
	bool isArrived = false;//ゴールに到着したかどうか

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