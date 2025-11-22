#pragma once
#include "Scene.h"
/// <summary>
/// GameScene0クラス(仮置きでいったん作った奴)
/// </summary>
class GameScene0 : public Scene //Sceneから継承したGameScene0
{
private:

	int frame_;
	
	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のGameScene0の状態に合わせて
	// 切り替えていく
	void FadeInUpdate(Input&);	// フェードインUpdate
	void NormalUpdate(Input&);	// 通常Update
	void FadeOutUpdate(Input&);	// フェードアウトUpdate
	using UpdateFunc_t = void(GameScene0::*)(Input&);
	UpdateFunc_t update_;	// Updateメンバ関数を代入できるメンバ関数ポインタ


	void NormalDraw();	// 通常Draw
	void FadeDraw();	// フェードDraw
	using DrawFunc_t = void(GameScene0::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	GameScene0(SceneController& controller);
	void Update(Input& input) override;
	void Draw()override;
};

