#pragma once
#include "Scene.h"
class TitleScene :  public Scene	// Sceneから継承したTitleScene
{
private:
	int titleH_;	// タイトル画像ハンドル
	int titleLogoH_;	// タイトルロゴハンドル
	int frame_;	// 経過フレーム

	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のTitleSceneの状態に合わせて
	// 切り替えていく
	void FadeInUpdate(Input&);	// フェードインUpdate
	void NormalUpdate(Input&);	// 通常Update
	void FadeOutUpdate(Input&);	// フェードアウトUpdate
	using UpdateFunc_t = void(TitleScene::*)(Input&);
	UpdateFunc_t update_;	// Updateメンバ関数を代入できるメンバ関数ポインタ


	void NormalDraw();	// 通常Draw
	void FadeDraw();	// フェードDraw
	using DrawFunc_t = void(TitleScene::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	TitleScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw()override;
};

