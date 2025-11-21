#pragma once
#include "Scene.h"
class GameScene : public Scene
{
public:
	GameScene(SceneController& controller);
	void Update(Input& input) override;
	void Draw() override;



private:
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	using UpdateFunc_t = void(GameScene::*)(Input&);
	UpdateFunc_t update_;	// Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t draw_;	// Draw系を受け取るメンバ関数ポインタ

};

