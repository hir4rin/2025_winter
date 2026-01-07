#pragma once
#include "Scene.h"
#include "Camera.h"
#include "StageUI.h"

class Player;
class Bg;
class Door;
class Effect;

class StageSelectScene :  public Scene
{
private:

	int m_frame;	// 経過フレーム

	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のTitleSceneの状態に合わせて
	// 切り替えていく
	void FadeInUpdate(Input&);	// フェードインUpdate
	void NormalUpdate(Input&);	// 通常Update
	void FadeOutUpdate(Input&);	// フェードアウトUpdate
	using UpdateFunc_t = void(StageSelectScene::*)(Input&);
	UpdateFunc_t update_;	// Updateメンバ関数を代入できるメンバ関数ポインタ


	void NormalDraw();	// 通常Draw
	void FadeDraw();	// フェードDraw
	void FadeInDraw(); //フェードイン
	void FadeOutDraw();//フェードアウト
	using DrawFunc_t = void(StageSelectScene::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	StageSelectScene(SceneController& controller,PlayerType type,int hp,int Life);
	void Update(Input& input) override;
	void Draw()override;
private:
	int m_handle = -1;
	int selectIndex = 0;
	std::shared_ptr<Player> m_pPlayer;
	//エフェクト
	std::vector<std::shared_ptr<Effect>> m_pEffects;

	Bg* m_pBg;
	Camera camera;//カメラのためのオブジェクト
	StageUI stageUI;//UI

	std::vector<std::shared_ptr<Door>> m_doors;
};

