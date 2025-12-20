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
	constexpr int kPlayerDuration = 60;
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
	const auto& wsize = Application::GetInstance().GetWindowSize();

	m_frame++;
	m_playerFrame++;

	Vec2 fPos = { wsize.w * 2.0f / 10.0f,wsize.h / 2.0f+ 200.0f };
	Vec2 sPos = { wsize.w * 5.0f / 10.0f,wsize.h / 2.0f + 200.0f };
	Vec2 tPos = { wsize.w * 8.0f / 10.0f,wsize.h / 2.0f + 200.0f };



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
			m_lights.push_back(std::make_shared<TitleLightEffect>(Vec2{ wsize.w * 2.0f / 10.0f,wsize.h / 2.0f }));
		}
		else if (playerSelect == 1)
		{
			m_pPlayer->ChangePos() = sPos;
			m_lights.push_back(std::make_shared<TitleLightEffect>(Vec2{ wsize.w * 5.0f / 10.0f,wsize.h / 2.0f }));
		}
		else if (playerSelect == 2)
		{
			m_pPlayer->ChangePos() = tPos;
			m_lights.push_back(std::make_shared<TitleLightEffect>(Vec2{ wsize.w * 8.0f / 10.0f,wsize.h / 2.0f }));
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




	DrawFormatString(1920*4/10, 1080 *3/4, 0xffffff, "Title Scene: Press 'ANY BUTTON' to Start");
	const auto& wsize = Application::GetInstance().GetWindowSize();
	if (m_frame > kPlayerDuration * 10)
	DrawRotaGraph(wsize.w / 2, wsize.h* 1/ 3, 0.8f, 0.0f, m_titleH, true);

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
	draw_ = &TitleScene::FadeDraw;
	frame_ = fade_interval;

	m_titleH = LoadGraph("data/TitleLogo.png");


	m_pPlayer = std::make_shared<Player>(PlayerType::Normal,100,Vec2{ wsize.w * 5.0f / 10.0f,wsize.h / 2.0f + 200.0f });


	m_stars.push_back(std::make_shared<TitleStar>(Vec2{ 400,800 }));
	m_lights.push_back(std::make_shared<TitleLightEffect>(Vec2{ wsize.w * 5.0f / 10.0f,wsize.h / 2.0f }));
	

}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}