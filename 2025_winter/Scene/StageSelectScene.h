#pragma once
#include "Scene.h"
class StageSelectScene :  public Scene
{
private:

	int frame_;	// 経過フレーム

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
	using DrawFunc_t = void(StageSelectScene::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	StageSelectScene(SceneController& controller,int Life);
	void Update(Input& input) override;
	void Draw()override;
private:
	int m_handle = -1;
	int selectIndex = 0;

};

