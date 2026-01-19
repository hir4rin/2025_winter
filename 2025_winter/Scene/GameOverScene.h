#pragma once
#include "Scene.h"
#include "Camera.h"
#include <memory>
#include <cassert>


class Player;
class Item;

enum class GameOverSelect
{
	Continue,
	End
};

class GameoverScene : public Scene
{

private:
	int frame_;	// 経過フレーム
	int timer;//画像をずらす用のタイマー
	int m_bgHandle = -1; // 背景画像ハンドル
	int m_LogoHandle = -1; // ロゴ画像ハンドル
	int m_LogoHandle2 = -1; // ロゴ画像ハンドル2
	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のGameoverSceneの状態に合わせて
	// 切り替えていく
	void FadeInUpdate(Input&);	// フェードインUpdate
	void NormalUpdate(Input&);	// 通常Update
	void FadeOutUpdate(Input&);	// フェードアウトUpdate
	void FadeOutUpdate2(Input&);	// フェードアウト2Update
	using UpdateFunc_t = void(GameoverScene::*)(Input&);
	UpdateFunc_t update_;	// Updateメンバ関数を代入できるメンバ関数ポインタ

	void NormalDraw();	// 通常Draw
	void FadeInDraw();	// フェードDraw
	void FadeOutDraw();	// フェードDraw
	using DrawFunc_t = void(GameoverScene::*)();
	DrawFunc_t draw_;	// Drawメンバ関数を代入できるメンバ関数ポインタ

public:
	GameoverScene(SceneController& controller,PlayerType type);
	~GameoverScene();
	void Update(Input& input) override;
	void Draw() override;

private:
	std::shared_ptr<Player> m_pPlayer;
	Camera camera;//カメラのためのオブジェクト
	//セレクト用
	GameOverSelect m_select = GameOverSelect::Continue;
	float drawX = 0.0f;
	float drawY = 0.0f;


	//演出用のアイテム
	std::shared_ptr<Item> m_pDroppedItem;

	int m_circleHandle = -1;
	int m_shadowHandle = -1;
private:
	int m_fontHandle;// フォントハンドル


};

