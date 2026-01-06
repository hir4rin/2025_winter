#pragma once
#include "Scene.h"
class GameoverScene : public Scene
{

private:
	int frame_;	// 経過フレーム

	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のGameoverSceneの状態に合わせて
	// 切り替えていく
	void FadeInUpdate(Input&);	// フェードインUpdate
	void NormalUpdate(Input&);	// 通常Update
	void FadeOutUpdate(Input&);	// フェードアウトUpdate
	using UpdateFunc_t = void(GameoverScene::*)(Input&);
	UpdateFunc_t update_;	// Updateメンバ関数を代入できるメンバ関数ポインタ

	void NormalDraw();	// 通常Draw
	void FadeInDraw();	// フェードDraw
	void FadeOutDraw();	// フェードDraw
	using DrawFunc_t = void(GameoverScene::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	GameoverScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;

private:




};

