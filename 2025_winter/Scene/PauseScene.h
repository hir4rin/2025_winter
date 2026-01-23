#pragma once
#include "Scene.h"
#include <vector>
#include <map>
#include <string>
#include <functional>	// ラムダ式を格納するのに必要

/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
private:
	// メニュー名と実行内容の対応テーブル
	std::map < std::string, std::function<void(Input&)>> execTable_;
	int frame_ = 0;
	int selectIndex_ = 0;	// 現在選択中のインデックス

	int yesNoIndex_ = 1;	// Noがデフォルトですが、yes=0,no=1とします。
	// yes/noの後で実行してほしい内容
	std::function<void()> yesRequestFunction_ = []() {};

	using UpdateFunc_t = void(PauseScene::*)(Input& input);
	UpdateFunc_t update_;
	void AppearUpdate(Input& input);
	void NormalUpdate(Input& input);
	void DisappearUpdate(Input& input);

	void YesNoDialogUpdate(Input& input);
	void YesNoDialogUpdate2(Input& input);
	void ConfigUpdate(Input& input);
	void VolumeSetUpdate(Input& input);

	using DrawFunc_t = void(PauseScene::*)();
	DrawFunc_t draw_;
	void IntervalDraw();
	void NormalDraw();
	void ConfigDraw();

	std::vector<std::string> menuList_;	// ポーズ画面中に表示するメニューリスト
	std::vector<std::string> menuList2_;	// 2ページ目に表示するメニューリスト
	void DrawMenu();//1ぺージ目の表示
	void DrawMenuPage2();//2ページ目の表示

	void YesNoDialogDraw();
	void YesNoDialogDraw2();
public:
	PauseScene(SceneController& controller);
	virtual ~PauseScene();
	void Update(Input& input) override;
	void Draw() override;
private:
	int m_LogoHandle = -1;
private:
	int m_fontHandle;// フォントハンドル
	float offsetY(int index);// ・・・の文字のYオフセット計算
	int m_frame;//タイマー
	int m_frame2;//【】を動かすようのタイマー
	int m_displayBGM;
	int m_displaySE;
	float m_volumeRate = 255.0f / 100.0f;//100で割って、255をかける
	float m_seRate = 1.4f;

};

