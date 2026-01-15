#pragma once
#include "Scene.h"

class TitleStar;
class TitleLightEffect;
class Player;

class TitleScene :  public Scene	// Sceneから継承したTitleScene
{
private:
	
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
	virtual ~TitleScene();
	void Update(Input& input) override;
	void Draw()override;
private:
	int m_frame;
	int m_playerFrame = 0;
	 int playerSelect = 0;

	int m_titleH = -1;
	int m_backH = -1;
	int m_handleStar = -1;
	std::vector<std::shared_ptr<TitleStar>> m_stars;
	std::vector<std::shared_ptr<TitleLightEffect>> m_lights;
	std::shared_ptr<Player> m_pPlayer;
private:
	int m_fontHandle;// フォントハンドル


	
};

