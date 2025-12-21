#include "TitleScene.h"
#include "DxLib.h"
#include "Player.h"
#include "TitleStar.h"
#include "TitleLightEffect.h"
#include "../input.h"
#include "StageSelectScene.h"
#include "SceneController.h"
#include "../Application.h"

constexpr int fade_interval = 60;

namespace
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	constexpr int kPlayerDuration = 45;

	Vec2 starPos = Vec2{ wsize.w / 2.0f, wsize.h * 3.0f / 10.0f };


	
	Vec2 fPos = { wsize.w * 2.0f / 10.0f,wsize.h / 2.0f + 415.0f };
	Vec2 fPos2 = { wsize.w * 2.0f / 10.0f,wsize.h / 2.0f + 200.0f };
	Vec2 fPos3 = { wsize.w * 2.0f / 10.0f,wsize.h / 2.0f + 200.0f+ 250.0f };

	Vec2 sPos = { wsize.w * 5.0f / 10.0f,wsize.h / 2.0f + 415.0f };
	Vec2 sPos2 = { wsize.w * 5.0f / 10.0f,wsize.h / 2.0f + 200.0f };
	Vec2 sPos3 = { wsize.w * 5.0f / 10.0f,wsize.h / 2.0f + 200.0f + 250.0f };

	Vec2 tPos = { wsize.w * 8.0f / 10.0f,wsize.h / 2.0f + 415.0f };
	Vec2 tPos2 = { wsize.w * 8.0f / 10.0f,wsize.h / 2.0f + 200.0f };
	Vec2 tPos3= { wsize.w * 8.0f / 10.0f,wsize.h / 2.0f + 200.0f + 250.0f };
}


void TitleScene::FadeInUpdate(Input&)
{
	if (frame_-- <= 0)
	{
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	

	m_frame++;
	m_playerFrame++;

	



	for (auto& star : m_stars)
	{
		if (!star)continue;
		star->Update();
	}

	//死んでるやつを消す
	m_lights.erase(
		std::remove_if(
			m_lights.begin(), m_lights.end(),
			[](const std::shared_ptr<TitleLightEffect>& light)
			{
				return light->GetIsDead();
			}),
		m_lights.end()
	);

	for (auto& light : m_lights)
	{
		if (!light)continue;
		light->Update();
	}


	if (m_playerFrame  >=  kPlayerDuration)
	{
		m_playerFrame = 0;

		
		 

		if (m_pPlayer->GetType() == PlayerType::Normal)
		{
			m_pPlayer->ChangeBurning();
			playerSelect++;
		}
		else if (m_pPlayer->GetType() == PlayerType::Burning)
		{
			m_pPlayer->ChangeFrozen();
			playerSelect++;
		}
		else if (m_pPlayer->GetType() == PlayerType::Frozen)
		{
			m_pPlayer->ChangeArcher();
			playerSelect++;
		}
		else if (m_pPlayer->GetType() == PlayerType::Archer)
		{
			m_pPlayer->ChangeNormal();
			playerSelect++;
		}

		playerSelect = playerSelect % 3;

		if (playerSelect == 0)
		{
			m_pPlayer->ChangePos() = fPos;
			m_lights.push_back(std::make_shared<TitleLightEffect>(fPos2 ,1));
			m_lights.push_back(std::make_shared<TitleLightEffect>(fPos3,2));
			m_lights.push_back(std::make_shared<TitleLightEffect>(fPos3,3));
		}
		else if (playerSelect == 1)
		{
			m_pPlayer->ChangePos() = sPos;
			m_lights.push_back(std::make_shared<TitleLightEffect>(sPos2,1));
			m_lights.push_back(std::make_shared<TitleLightEffect>(sPos3, 2));
			m_lights.push_back(std::make_shared<TitleLightEffect>(sPos3, 3));
		}
		else if (playerSelect == 2)
		{
			m_pPlayer->ChangePos() = tPos;
			m_lights.push_back(std::make_shared<TitleLightEffect>(tPos2,1));
			m_lights.push_back(std::make_shared<TitleLightEffect>(tPos3, 2));
			m_lights.push_back(std::make_shared<TitleLightEffect>(tPos3, 3));
		}

	}











	if (input.IsTriggered("ok") || input.IsTriggered("Attack")|| input.IsTriggered("Jump")|| input.IsTriggered("Copy")|| input.IsTriggered("CopyOut")|| input.IsTriggered("pause"))
	{
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frame_ = 0;	// フェードアウトの最初
		return;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (frame_++ >= fade_interval)
	{
		controller_.ChangeScene(std::make_shared<StageSelectScene>(controller_));
		return;
	}
}

void TitleScene::NormalDraw()
{
	//背景
	
	int x = 0;
	int y = 0;

	GetGraphSize(m_backH, &x, &y);

	DrawRectRotaGraph(wsize.w/2.0f, wsize.h/ 2.0f,
		0, 0,
		x, y,
		10.0f, 0.0f,
		m_backH, false);

	// 薄暗く
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140); // 好きな濃さ
	DrawBox(0, 0, screenWidth, screenHeight, 0x000000, true);
	//DrawBox(0, 0, screenWidth, screenHeight, 0x303030, true);//
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	for (auto& star : m_stars)
	{
		if (!star)continue;
		star->Draw();
	}

	for (auto& light : m_lights)
	{
		if (!light)continue;
		light->Draw();
	}

	if (m_pPlayer)m_pPlayer->Draw();




	DrawFormatString(1920*4/10, 1080 *1/2, 0xffffff, "Title Scene: Press 'ANY BUTTON' to Start");
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//とりあえず
	if (m_frame > kPlayerDuration * 7)
	DrawRotaGraph(wsize.w / 2, wsize.h* 1/ 5 + 50, 0.5f, 0.0f, m_titleH, true);

	

}

void TitleScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, titleH_, true);
	//DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, titleLogoH_, true);
	//// 値の範囲をいったん0.0~1.0にしておくといろいろと扱いやすくなります
	auto rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,255 * rate); // αブレンド
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);	// 画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドしない
}

TitleScene::TitleScene(SceneController& controller) : Scene(controller)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	/*titleH_ = LoadGraph(L"data/title.png");
	titleLogoH_ = LoadGraph(L"data/game_title.png");*/
	update_ = &TitleScene::FadeInUpdate;
	draw_ = &TitleScene::NormalDraw;
	frame_ = fade_interval;

	m_titleH = LoadGraph("data/TitleLogo.png");
	m_backH = LoadGraph("data/TitleBack.png");
	//初期化
	playerSelect = 1;
	m_frame = 0;

	m_pPlayer = std::make_shared<Player>(PlayerType::Normal,100,sPos);


	m_stars.push_back(std::make_shared<TitleStar>(starPos,350.0f));
	m_lights.push_back(std::make_shared<TitleLightEffect>(sPos2,1));
	m_lights.push_back(std::make_shared<TitleLightEffect>(sPos3,2));
	m_lights.push_back(std::make_shared<TitleLightEffect>(sPos3,3));
	

}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}