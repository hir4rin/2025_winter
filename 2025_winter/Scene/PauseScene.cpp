#include "PauseScene.h"
#include "../Input.h"
#include "SceneController.h"
#include "DxLib.h"
#include "../Application.h"
//#include "KeyConfigScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"

namespace
{
constexpr int frame_margin = 10;	// 枠が画面端からどれくらい離れてるか
constexpr int appear_interval = 10;	// 枠が出現するまでのフレーム数
constexpr int menu_row_height = 130;	// メニューの行の高さ
constexpr int menu_left_margin = 100;	// メニュー枠からの左余白
constexpr int menu_top_margin = 120;	// メニュー枠からの上余白
constexpr int yes_no_dialig_yes = 0;	// yesnodialogでyes
constexpr int yes_no_dialig_no = 1;		// yesnodialogでno

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;
}

void PauseScene::AppearUpdate(Input& input)
{
	if (frame_ == appear_interval)
	{
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
	frame_++;
}

void PauseScene::NormalUpdate(Input& input)
{
	//タイマー更新
	m_frame++;
	if (input.IsTriggered("pause"))
	{
		//戻った時の音
		controller_.PopScene();	// この時点で自分は解放されている
		return;
	}
	if (input.IsTriggered("up"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		selectIndex_ = (selectIndex_ + menuList_.size() - 1) % menuList_.size();
	}
	if (input.IsTriggered("down"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		selectIndex_ = (selectIndex_ + 1) % menuList_.size();
	}
	if ( input.IsTriggered("Jump"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("ok2");
		// 現在選択中のメニューアイテム名を取得する
		auto& menuName = menuList_[selectIndex_];
		// そのメニューアイテムの名前に対応付けられたラムダ式を実行する
		execTable_[menuName](input);
		return;
	}
	if (input.IsTriggered("ok"))
	{
		//１つ前に戻る
		controller_.PopScene();	// この時点で自分は解放されている
		return;
	}
}

void PauseScene::DisappearUpdate(Input& input)
{
	//今は使っていない
	if (frame_ == 0)
	{
		controller_.PopScene();	// この時点で自分は解放されている
	}
	frame_--;
}

void PauseScene::YesNoDialogUpdate(Input& input)
{
	//タイマー更新
	m_frame++;
	if (input.IsTriggered("left") || input.IsTriggered("right"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		yesNoIndex_ = (yesNoIndex_ + 1) % 2;
		return;
	}
	if (input.IsTriggered("ok") || input.IsTriggered("Jump"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("ok2");
		if (yesNoIndex_ == yes_no_dialig_yes)	// yesのとき
		{
			yesRequestFunction_();
			return;
		}
		else	// noのとき
		{
			yesRequestFunction_ = [](){};
			update_ = &PauseScene::NormalUpdate;
			draw_ = &PauseScene::NormalDraw;
		}
	}
}

void PauseScene::YesNoDialogUpdate2(Input& input)
{
	//タイマー更新
	m_frame++;
	if (input.IsTriggered("left") || input.IsTriggered("right"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		yesNoIndex_ = (yesNoIndex_ + 1) % 2;
		return;
	}
	if (input.IsTriggered("ok") || input.IsTriggered("Jump"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("ok2");
		if (yesNoIndex_ == yes_no_dialig_yes)	// yesのとき
		{
			yesRequestFunction_();
			return;
		}
		else	// noのとき
		{
			yesRequestFunction_ = []() {};
			update_ = &PauseScene::ConfigUpdate;
			draw_ = &PauseScene::ConfigDraw;
		}
	}
}

void PauseScene::ConfigUpdate(Input& input)
{
	//タイマー更新
	m_frame++;
	if (input.IsTriggered("pause"))
	{
		//戻った時の音
		controller_.PopScene();	// この時点で自分は解放されている
		return;
	}
	if (input.IsTriggered("up"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		selectIndex_ = (selectIndex_ + menuList_.size() - 1) % menuList_.size();
	}
	if (input.IsTriggered("down"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("cursor");
		selectIndex_ = (selectIndex_ + 1) % menuList_.size();
	}
	if (input.IsTriggered("Jump"))
	{
		//SE再生
		Application::GetInstance().GetSoundManager().PlaySE("ok2");
		// 現在選択中のメニューアイテム名を取得する
		auto& menuName = menuList2_[selectIndex_];
		// そのメニューアイテムの名前に対応付けられたラムダ式を実行する
		execTable_[menuName](input);
		return;
	}
	if (input.IsTriggered("ok"))
	{
		//１つ前に戻る
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
	}
	
}

void PauseScene::VolumeSetUpdate(Input& input)
{
	m_frame++;
	m_frame2++;
	if (input.IsTriggered("left"))
	{
		if (selectIndex_ == 0)//BGM
		{
			m_displayBGM -= 5;
			if (m_displayBGM <= 0)m_displayBGM = 0;
			//BGMセット
			Application::GetInstance().GetSoundManager().SetBgmVolume(m_displayBGM * m_volumeRate);
		}
		if (selectIndex_ == 1)//SE
		{
			m_displaySE -= 5;
			if (m_displaySE <= 0)m_displaySE = 0;
			int bet = m_displaySE * m_seRate;
			if (bet <= 0)bet = 0;
			//SEセット
			Application::GetInstance().GetSoundManager().SetSEVolume(bet * m_volumeRate);
		}
	}
	if (input.IsTriggered("right"))
	{
		if (selectIndex_ == 0)//BGM
		{
			m_displayBGM += 5;
			if (m_displayBGM >= 100)m_displayBGM = 100;
			//BGMセット
			Application::GetInstance().GetSoundManager().SetBgmVolume(m_displayBGM * m_volumeRate);
		}
		if (selectIndex_ == 1)//SE
		{
			m_displaySE += 5;
			if (m_displaySE >= 100)m_displaySE = 100;
			int bet = m_displaySE * m_seRate;
			if (bet >= 100)bet = 100;
			//SEセット
			Application::GetInstance().GetSoundManager().SetSEVolume(bet * m_volumeRate);
		}
	}
	if (input.IsTriggered("ok") || input.IsTriggered("Jump"))
	{
		update_ = &PauseScene::ConfigUpdate;
		draw_ = &PauseScene::ConfigDraw;
	}
}

void PauseScene::IntervalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	int center_y = wsize.h / 2;	// 画面の真ん中のY座標
	int center_x = wsize.w / 2;	// 画面の真ん中のX座標
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);	// 表示割合

	int frame_height = (wsize.h - frame_margin) - center_y;	// 最終的なポーズ枠の高さ(真ん中から枠までの距離)
	int frame_width = (wsize.w - frame_margin) - center_x;
	frame_height *= rate;	// 表示する高さを割合で調節
	//frame_width *= rate;

	DrawRotaGraph(kScreenWidth / 2.0f, kScreenHeight / 2.0f, 1.0f, 0.0f, m_LogoHandle, true);


}

void PauseScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//タイトル
	DrawRotaGraph(kScreenWidth / 2.0f, kScreenHeight / 2.0f,1.0f,0.0f, m_LogoHandle, true);
	//ポーズ中の文字
	DrawStringToHandle(kScreenWidth / 2.0f - 140.0f,kScreenHeight *4.0f/ 20.0f, "ポーズちゅう", GetColor(0, 0, 0), m_fontHandle);
	for (int i = 0; i < 3; i++)
	{
		float drawY = offsetY(i);
		//・・・の文字
		DrawStringToHandle(kScreenWidth *2.0f/ 3.0f-200.0f + 40 * i, kScreenHeight * 4.0f / 20.0f+10.0f + drawY, "・", GetColor(0, 0, 0), m_fontHandle);
	}
	DrawMenu();
}

void PauseScene::ConfigDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//タイトル
	DrawRotaGraph(kScreenWidth / 2.0f, kScreenHeight / 2.0f, 1.0f, 0.0f, m_LogoHandle, true);
	//ポーズ中の文字
	DrawStringToHandle(kScreenWidth / 2.0f - 140.0f, kScreenHeight * 4.0f / 20.0f, "せってい", GetColor(0, 0, 0), m_fontHandle);
	for (int i = 0; i < 3; i++)
	{
		float drawY = offsetY(i);
		//・・・の文字
		DrawStringToHandle(kScreenWidth * 2.0f / 3.0f - 200.0f + 40 * i, kScreenHeight * 4.0f / 20.0f + 10.0f + drawY, "・", GetColor(0, 0, 0), m_fontHandle);
	}
	DrawMenuPage2();
}

void PauseScene::DrawMenu()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float baseY = wsize.w * 1.0f / 8.0f;
	int menuStartX = wsize.w * 1.0f / 3.0f;
	int indicatorX = menuStartX - 60;
	int menuY = wsize.h * 1.0f/8.0f;
	for (int idx = 0; idx < menuList_.size(); idx++)
	{
		int offsetX = 0;
		uint32_t col = 0xffffff;
		if (idx == selectIndex_)
		{
			float rad = m_frame / 100.0f * DX_PI;
			float drawX = sin(rad) * -15.0f;
			DrawStringToHandle(indicatorX + drawX, baseY + menuY, "→", GetColor(0, 0, 0), m_fontHandle);
			offsetX = 10;
			col = GetColor(128, 255, 192);
		}
		std::string text = menuList_[idx];
		//DrawFormatString(menuStartX + offsetX, menuY, col, "%s", menuList_[idx].c_str());
		DrawStringToHandle(menuStartX + offsetX, baseY +menuY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
		menuY += menu_row_height;
	}
}

void PauseScene::DrawMenuPage2()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float baseY = wsize.w * 1.0f / 8.0f;
	int menuStartX = wsize.w * 1.0f / 3.0f;
	int indicatorX = menuStartX - 60;
	int menuY = wsize.h * 1.0f / 8.0f;
	std::string num1 = std::to_string(m_displayBGM);
	std::string num2 = std::to_string(m_displaySE);
	if (update_ == &PauseScene::VolumeSetUpdate)//sin波の揺れ用
	{
		float rad = m_frame2 / 30.0f * DX_PI;
		float drawX = sin(rad) * -7.f;
		if (selectIndex_ == 0)//BGM
		{
			//BGM
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + drawX, baseY + menuY, "【", GetColor(255, 180, 210), m_fontHandle);
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 100, baseY + menuY, num1.c_str(), GetColor(255, 180, 210), m_fontHandle);
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 200 - drawX, baseY + menuY, "】", GetColor(255, 180, 210), m_fontHandle);
		}
		else if (selectIndex_ == 1)//SE
		{
			//SE
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + drawX, baseY + menuY + menu_row_height, "【", GetColor(255, 180, 210), m_fontHandle);
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 100, baseY + menuY + menu_row_height, num2.c_str(), GetColor(255, 180, 210), m_fontHandle);
			DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 200 - drawX, baseY + menuY + menu_row_height, "】", GetColor(255, 180, 210), m_fontHandle);
		}
		
	}
	else
	{
		//BGM
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f, baseY + menuY, "【", GetColor(0, 0, 0), m_fontHandle);
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 100, baseY + menuY, num1.c_str(), GetColor(0, 0, 0), m_fontHandle);
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 200, baseY + menuY, "】", GetColor(0, 0, 0), m_fontHandle);
		//SE
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f, baseY + menuY + menu_row_height, "【", GetColor(0, 0, 0), m_fontHandle);
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 100, baseY + menuY + menu_row_height, num2.c_str(), GetColor(0, 0, 0), m_fontHandle);
		DrawStringToHandle(indicatorX + wsize.w * 1.0f / 4.0f + 200, baseY + menuY + menu_row_height, "】", GetColor(0, 0, 0), m_fontHandle);
	}
	
	for (int idx = 0; idx < menuList2_.size(); idx++)
	{
		int offsetX = 0;
		uint32_t col = 0xffffff;
		if (idx == selectIndex_)
		{
			float rad = m_frame / 100.0f * DX_PI;
			float drawX = sin(rad) * -15.0f;
			DrawStringToHandle(indicatorX + drawX, baseY + menuY, "→", GetColor(0, 0, 0), m_fontHandle);
			offsetX = 10;
			col = GetColor(128, 255, 192);
		}
		std::string text = menuList2_[idx];
		//DrawFormatString(menuStartX + offsetX, menuY, col, "%s", menuList_[idx].c_str());
		if (update_ == &PauseScene::VolumeSetUpdate && idx == selectIndex_)//選択中の文字の色を変える
		{
			DrawStringToHandle(menuStartX + offsetX, baseY + menuY, text.c_str(), GetColor(255, 180, 210), m_fontHandle);
		}
		else
		{
			DrawStringToHandle(menuStartX + offsetX, baseY + menuY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
		}
		menuY += menu_row_height;
	}

}

void PauseScene::YesNoDialogDraw()
{
	//ポーズ画面の土台
	const auto& wsize = Application::GetInstance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//タイトル
	DrawRotaGraph(kScreenWidth / 2.0f, kScreenHeight / 2.0f, 1.0f, 0.0f, m_LogoHandle, true);
	//ポーズ中の文字
	DrawStringToHandle(kScreenWidth / 2.0f - 140.0f, kScreenHeight * 4.0f / 20.0f, "ポーズちゅう", GetColor(0, 0, 0), m_fontHandle);
	for (int i = 0; i < 3; i++)
	{
		float drawY = offsetY(i);
		//・・・の文字
		DrawStringToHandle(kScreenWidth * 2.0f / 3.0f - 200.0f + 40 * i, kScreenHeight * 4.0f / 20.0f + 10.0f + drawY, "・", GetColor(0, 0, 0), m_fontHandle);
	}
	//YesNoDialogの内容
	 int yes_no_dialog_height = Application::GetInstance().GetWindowSize().w * 1.0f / 8.0f;
	constexpr int yes_no_dialog_width = 300;
	const int centerX = Application::GetInstance().GetWindowSize().w *1.0f/ 3.0f;
	const int centerY = Application::GetInstance().GetWindowSize().h * 2.0f / 8.0f;
	float baseY = Application::GetInstance().GetWindowSize().w * 1.0f / 8.0f;

	const int dialog_left = centerX - 150;

	int y = centerY;	// 画面中心から文字サイズの半分引く
	int x = dialog_left + 250;	// はい、いいえが真ん中に来るように
	std::array<std::string, 2> answers = { "はい","いいえ" };

	// ダイアログタイトルを表示
	/*DrawFormatString(centerX + 600, centerY - yes_no_dialog_height / 2 + 10,
		0xffffff,"%s", menuList_[selectIndex_].c_str());*/
	std::string text = menuList_[selectIndex_];
	if (selectIndex_ == 2)//ステージセレクトに戻る
	{
		DrawStringToHandle(centerX + 50, centerY - yes_no_dialog_height / 2 + baseY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
	}
	else//その他
	{
		DrawStringToHandle(centerX + 150, centerY - yes_no_dialog_height / 2 + baseY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
	}
	for (int idx = 0; idx < 2; idx++)
	{
		uint32_t col = 0xffffff;
		if (yesNoIndex_ == idx)
		{
			float rad = m_frame / 100.0f * DX_PI;
			float drawX = sin(rad) * -15.0f;
			DrawStringToHandle(x-60+drawX , baseY + y, "→", GetColor(0, 0, 0), m_fontHandle);
			col = 0xff4444;
		}
		//はい、いいえの文字
		std::string text = answers[idx];
		DrawStringToHandle(x, baseY + y, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
		x += 300;
	}
}

void PauseScene::YesNoDialogDraw2()
{
	//ポーズ画面の土台
	const auto& wsize = Application::GetInstance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//タイトル
	DrawRotaGraph(kScreenWidth / 2.0f, kScreenHeight / 2.0f, 1.0f, 0.0f, m_LogoHandle, true);
	//ポーズ中の文字
	DrawStringToHandle(kScreenWidth / 2.0f - 140.0f, kScreenHeight * 4.0f / 20.0f, "せってい", GetColor(0, 0, 0), m_fontHandle);
	for (int i = 0; i < 3; i++)
	{
		float drawY = offsetY(i);
		//・・・の文字
		DrawStringToHandle(kScreenWidth * 2.0f / 3.0f - 200.0f + 40 * i, kScreenHeight * 4.0f / 20.0f + 10.0f + drawY, "・", GetColor(0, 0, 0), m_fontHandle);
	}
	//YesNoDialogの内容
	int yes_no_dialog_height = Application::GetInstance().GetWindowSize().w * 1.0f / 8.0f;
	constexpr int yes_no_dialog_width = 300;
	const int centerX = Application::GetInstance().GetWindowSize().w * 1.0f / 3.0f;
	const int centerY = Application::GetInstance().GetWindowSize().h * 2.0f / 8.0f;
	float baseY = Application::GetInstance().GetWindowSize().w * 1.0f / 8.0f;

	const int dialog_left = centerX - 150;

	int y = centerY;	// 画面中心から文字サイズの半分引く
	int x = dialog_left + 250;	// はい、いいえが真ん中に来るように
	std::array<std::string, 2> answers = { "はい","いいえ" };

	// ダイアログタイトルを表示
	/*DrawFormatString(centerX + 600, centerY - yes_no_dialog_height / 2 + 10,
		0xffffff,"%s", menuList_[selectIndex_].c_str());*/
	std::string text = menuList2_[selectIndex_];
	if (selectIndex_ == 2)//ステージセレクトに戻る
	{
		DrawStringToHandle(centerX + 50, centerY - yes_no_dialog_height / 2 + baseY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
	}
	else//その他
	{
		DrawStringToHandle(centerX + 150, centerY - yes_no_dialog_height / 2 + baseY, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
	}
	for (int idx = 0; idx < 2; idx++)
	{
		uint32_t col = 0xffffff;
		if (yesNoIndex_ == idx)
		{
			float rad = m_frame / 100.0f * DX_PI;
			float drawX = sin(rad) * -15.0f;
			DrawStringToHandle(x - 60 + drawX, baseY + y, "→", GetColor(0, 0, 0), m_fontHandle);
			col = 0xff4444;
		}
		//はい、いいえの文字
		std::string text = answers[idx];
		DrawStringToHandle(x, baseY + y, text.c_str(), GetColor(0, 0, 0), m_fontHandle);
		x += 300;
	}
}

PauseScene::PauseScene(SceneController& controller) : 
	Scene(controller),
	update_(&PauseScene::NormalUpdate),
	draw_(&PauseScene::NormalDraw),
	m_fontHandle(-1),
	m_frame(0),
	m_frame2(0)
{
	m_LogoHandle = LoadGraph("data/Pause.png");
	//フォントの生成
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 48, 6, -1);
	//音量
	m_displayBGM = (Application::GetInstance().GetSoundManager().GetBgmVolume()) / m_volumeRate;
	m_displaySE = (Application::GetInstance().GetSoundManager().GetBgmVolume()) / m_volumeRate;
	menuList_ = {
		"ゲームにもどる",
		//"キーコンフィグ",
		"せってい",
		"ステージセレクトにもどる",
		"ゲームをおわる"
	};
	menuList2_ = {
		"BGM",
		"SE",
		"デフォルトにもどす",
		"もどる"
	};

	// メニューで選ばれる文字列と実行される内容のペアを定義
	execTable_["ゲームにもどる"] = [this](Input&) {
		controller_.PopScene();	// この時点で自分は解放されている
		};
	//execTable_[L"キーコンフィグ"] = [this](Input& input) {
	//	controller_.PushScene(std::make_shared<KeyConfigScene>(controller_, input));
	//	};
	execTable_["せってい"] = [this](Input&) {
		update_ = &PauseScene::ConfigUpdate;
		draw_ = &PauseScene::ConfigDraw;
		yesRequestFunction_ = [this]() {
		
			};
		};
	execTable_["BGM"] = [this](Input&) {
		update_ = &PauseScene::VolumeSetUpdate;
		draw_ = &PauseScene::ConfigDraw;
		//yesRequestFunction_ = [this]() {
		//	//BGM設定の処理
		//	};
		};
	execTable_["SE"] = [this](Input&) {
		update_ = &PauseScene::VolumeSetUpdate;
		draw_ = &PauseScene::ConfigDraw;
		//yesRequestFunction_ = [this]() {
		//	//SE設定の処理
		//	};
		};
	execTable_["デフォルトにもどす"] = [this](Input&) {
		update_ = &PauseScene::YesNoDialogUpdate2;
		draw_ = &PauseScene::YesNoDialogDraw2;
		yesRequestFunction_ = [this]() {
			//デフォルトに戻す処理
			m_displayBGM = 50;
			m_displaySE = 50;
			Application::GetInstance().GetSoundManager().SetBgmVolume(m_displayBGM * m_volumeRate);
			Application::GetInstance().GetSoundManager().SetSEVolume(m_displayBGM * m_volumeRate);
			update_ = &PauseScene::ConfigUpdate;
			draw_ = &PauseScene::ConfigDraw;
			return;
			};
		};
	execTable_["もどる"] = [this](Input&) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		yesRequestFunction_ = [this]() {
			//デフォルトに戻す処理
			};
		};
	execTable_["ステージセレクトにもどる"] = [this](Input&) {
		update_ = &PauseScene::YesNoDialogUpdate;
		draw_ = &PauseScene::YesNoDialogDraw;
		yesRequestFunction_ = [this](){
			//そして、このシーンに変える
			controller_.ResetScene(std::make_shared<StageSelectScene>(controller_,
				controller_.m_savePlayer.s_type,
				controller_.m_savePlayer.s_hp,
				controller_.m_savePlayer.s_Life));

			};
		};
	execTable_["ゲームをおわる"] = [this](Input&) {
		update_ = &PauseScene::YesNoDialogUpdate;
		draw_ = &PauseScene::YesNoDialogDraw;
		yesRequestFunction_ = [this]() {
			Application::GetInstance().GetSoundManager().PlayBgm("bgm");
			controller_.ResetScene(std::make_shared<TitleScene>(controller_));
			};
		};
}

PauseScene::~PauseScene()
{
	DeleteGraph(m_LogoHandle);
	//生成したフォントの削除
	DeleteFontToHandle(m_fontHandle);
}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}
float PauseScene::offsetY(int index)
{

	int startDelay = index * 8;   // 文字ごとの遅延
	int moveFrame = 30;          // 動く時間
	int waitFrame = 30;          // 止まる時間
	int cycle = moveFrame + waitFrame;



	//m_goalFrameはだんだん大きくなる
	int f = (m_frame - startDelay) % cycle;

	if (f < 0)
		return 0.0f;              // まだ開始していない

	if (f >= moveFrame)
		return 0.0f;              // 止まっている

	float t = (float)f / moveFrame * DX_PI;
	return sinf(t) * -15.0f;
}