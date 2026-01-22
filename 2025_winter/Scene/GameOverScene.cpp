#include "GameoverScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "SceneController.h"
#include "Application.h"

namespace
{
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;

	const Vec2 PlayerStartPos = Vec2{ kScreenWidth * 1.0f / 2.0f - 25,kScreenHeight * 1.0f / 2.0f + 70 };
	const Vec2 CirclePos = Vec2{ kScreenWidth * 1.0f / 2.0f,kScreenHeight * 1.0f / 2.0f + 100 };
	const Vec2 ShadowPos = Vec2{ kScreenWidth * 1.0f / 2.0f,kScreenHeight * 1.0f / 2.0f + 110 };


	//下の丸
	constexpr int kDaenW = 64;
	constexpr int kDaenH = 64;
	constexpr float kDaenScale = 2.0f;
	//その影
	constexpr int kShadowW = 100;
	constexpr int kShadowH = 100;
	constexpr float kShadowScale = 3.0f;

	//ロゴ
	constexpr int kLogoWidth = 600;
	constexpr int kLogoHeight = 250;
	//フェードインアウトの時間
	constexpr int fade_interval = 75;

}


GameoverScene::GameoverScene(SceneController& controller,PlayerType type) : Scene(controller)
, frame_(0),
m_fontHandle(-1),
timer(0)
{
	//SE止まる
	Application::GetInstance().GetSoundManager().StopSE("yarareSE");
	//bgm再生
	Application::GetInstance().GetSoundManager().PlayBgm("bgmGameOverScene");
	//背景
	m_bgHandle = LoadGraph("data/Back/GameOverBg.png");
	//ロゴ
	m_LogoHandle = LoadGraph("data/game.png");
	m_LogoHandle2 = LoadGraph("data/over.png");

	update_ = &GameoverScene::FadeInUpdate;
	draw_ = &GameoverScene::FadeInDraw;

	m_pPlayer = std::make_shared<Player>(type, PlayerStartPos,controller.GetEffekseerResourceManager());

	//画像
	{
		m_circleHandle = LoadGraph("data/daen.png");
		m_shadowHandle = LoadGraph("data/Game/Shadow.png");
	}
	//フォントの生成
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 48, 6, -1);

	InitCamera(camera,0);//カメラの初期化
}

GameoverScene::~GameoverScene()
{
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_LogoHandle);
	DeleteGraph(m_LogoHandle2);
	DeleteGraph(m_circleHandle);
	DeleteGraph(m_shadowHandle);
	//生成したフォントの削除
	DeleteFontToHandle(m_fontHandle);


}



void GameoverScene::FadeInUpdate(Input&)
{
	if (frame_++ >= fade_interval)
	{

		update_ = &GameoverScene::NormalUpdate;
		draw_ = &GameoverScene::NormalDraw;
		return;
	}
}
void GameoverScene::NormalUpdate(Input& input)
{
	timer++;
	m_pPlayer->GameOverUpdate();


	if(input.IsTriggered("up"))
	{
		if (m_select == GameOverSelect::Continue)
		{
			//ばつの音
			Application::GetInstance().GetSoundManager().PlaySE("cursorError");
		}
		else
		{
			Application::GetInstance().GetSoundManager().PlaySE("cursor");
		}

		m_select = GameOverSelect::Continue;
	}
	if(input.IsTriggered("down"))
	{
		if (m_select == GameOverSelect::End)
		{
			//ばつの音
			Application::GetInstance().GetSoundManager().PlaySE("cursorError");
		}
		else
		{
			Application::GetInstance().GetSoundManager().PlaySE("cursor");
		}
		m_select = GameOverSelect::End;
	}


	if (input.IsTriggered("Jump"))
	{
		//bgm再生
		Application::GetInstance().GetSoundManager().PlaySE("ok");
		if(m_select == GameOverSelect::Continue)
		{
			update_ = &GameoverScene::FadeOutUpdate;
			draw_ = &GameoverScene::NormalDraw;
			frame_ = 200;
			m_pPlayer->AnimChangeStandUp();
			return;
		}
		if(m_select == GameOverSelect::End)
		{
			controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
			return;
		}
	}
}
void GameoverScene::FadeOutUpdate(Input&)
{
	frame_--;
	timer++;
	m_pPlayer->GameOverStandUpUpdate(PlayerStartPos.y);
	if (m_pDroppedItem)//演出のアイテムのアップデート
	{
		m_pDroppedItem->DroppedUpdate();
		if (m_pDroppedItem->IsDead())
		{
			m_pDroppedItem = nullptr;
		}
	}

	//30フレーム以下になったらドロップ処理
	if (frame_ <= 30)
	{
		//アイテムドロップ処理
		{
			if (!(m_pPlayer->GetType() == PlayerType::Normal))
			{
				//SE再生
				Application::GetInstance().GetSoundManager().PlaySE("copyOut");
				//プレイヤーのタイプに応じて落とすアイテムを変える
				switch (m_pPlayer->GetType())
				{
				case PlayerType::Frozen:
					m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyWizard>());
					m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
					m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
					break;
				case PlayerType::Burning:
					m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyRider>());
					m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
					m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
					break;
				case PlayerType::Archer:
					m_pDroppedItem = std::make_shared<Item>(std::make_shared<EnemyArcher>());
					m_pDroppedItem->ChangePos() = m_pPlayer->GetPos();
					m_pDroppedItem->Setm_isRight(m_pPlayer->Getm_isRight());
					break;
				}
				//プレイヤーをノーマルに戻す
				m_pPlayer->ChangeNormal();

			}
		}
	}
	
	if (frame_ <= 0)
	{
		frame_ = fade_interval;
		update_ = &GameoverScene::FadeOutUpdate2;
		draw_ = &GameoverScene::FadeOutDraw;
	}
}
void GameoverScene::FadeOutUpdate2(Input&)
{
	m_pPlayer->GameOverStandUpUpdate(PlayerStartPos.y);
	if (frame_-- <= 0)
	{

		Application::GetInstance().GetSoundManager().PlayBgm("bgm");
		controller_.ChangeScene(std::make_shared<StageSelectScene>(controller_, PlayerType::Normal, 100, 2));
		return;
	}
	
}
void GameoverScene::NormalDraw()
{
	//背景

	int x = 0;
	int y = 0;

	GetGraphSize(m_bgHandle, &x, &y);
	//背景
	DrawRectRotaGraph(kScreenWidth / 2.0f + 50, kScreenHeight / 2.0f,
		0, 0,
		x, y,
		1.8f, 0.0f,
		m_bgHandle, false);

	DrawRectRotaGraph(CirclePos.x, CirclePos.y,
	kDaenW * 0, kDaenH * 0,//切り取り左上
	kDaenW, kDaenH,//切り取りの幅
	kDaenScale, 0, m_circleHandle, true);


	DrawRectRotaGraph(m_pPlayer->GetPos().x, ShadowPos.y - 20.0f,
	kShadowW * 0, kShadowH * 0,//切り取り左上
	kShadowW, kShadowH,//切り取りの幅
	kShadowScale, 0, m_shadowHandle, true);



	m_pPlayer->Draw(camera);
	if (m_pDroppedItem) m_pDroppedItem->DroppedDraw(camera);
	//選択肢
	DrawStringToHandle(kScreenWidth *2.3f/5.0f,kScreenHeight* 2.0f/3.0f, "Continue", GetColor(0, 0, 0), m_fontHandle);
	DrawStringToHandle(kScreenWidth * 2.3f / 5.0f,kScreenHeight* 2.0f/3.0f+100, "End", GetColor(0, 0, 0), m_fontHandle);
	if (m_select == GameOverSelect::Continue)
	{
		DrawStringToHandle(kScreenWidth * 2.0f / 5.0f, kScreenHeight * 2.0f / 3.0f, "→", GetColor(0, 0, 0), m_fontHandle);
	}
	else
	{
		DrawStringToHandle(kScreenWidth * 2.0f / 5.0f, kScreenHeight * 2.0f / 3.0f + 100, "→", GetColor(0, 0, 0), m_fontHandle);
	}
	//ふわふわ動かす
	drawY = sin(timer * 0.05f) * 10.0f;
	float drawY2 = sin(timer * 0.05f + DX_PI) * 10.0f;


	//ロゴ
	DrawRectRotaGraph(kScreenWidth / 3.0f, kScreenHeight * 1.0f / 3.0f+ drawY,
		kLogoWidth * 0, kLogoHeight * 0,//切り取り左上
		kLogoWidth, kLogoHeight,//切り取りの幅
		1.0f, 0, m_LogoHandle, true);
	//ロゴ2
	DrawRectRotaGraph(kScreenWidth* 2.0f/ 3.0f, kScreenHeight * 1.0f / 3.0f+drawY,
		kLogoWidth * 0, kLogoHeight * 0,//切り取り左上
		kLogoWidth, kLogoHeight,//切り取りの幅
		1.0f, 0, m_LogoHandle2, true);

}
void GameoverScene::FadeInDraw()
{
	NormalDraw();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (1.0f-rate));
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void GameoverScene::FadeOutDraw()
{
	NormalDraw();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (1.0f-rate));
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);

}

void GameoverScene::Draw()
{
	
	(this->*draw_)();

}