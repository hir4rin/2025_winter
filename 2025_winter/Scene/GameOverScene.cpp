#include "GameoverScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWizard.h"
#include "EnemyRider.h"
#include "EnemyArcher.h"
#include "TitleScene.h"
#include "SceneController.h"

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

}


GameoverScene::GameoverScene(SceneController& controller,PlayerType type) : Scene(controller)
, frame_(0)
{
	update_ = &GameoverScene::NormalUpdate;
	draw_ = &GameoverScene::NormalDraw;

	m_pPlayer = std::make_shared<Player>(type, PlayerStartPos);

	//画像
	{
		m_circleHandle = LoadGraph("data/daen.png");
		m_shadowHandle = LoadGraph("data/Game/Shadow.png");
	}


	InitCamera(camera,0);//カメラの初期化
}



void GameoverScene::FadeInUpdate(Input&)
{
}
void GameoverScene::NormalUpdate(Input& input)
{
	m_pPlayer->GameOverUpdate();



	if (input.IsTriggered("ok"))
	{
		update_ = &GameoverScene::FadeOutUpdate;
		draw_ = &GameoverScene::NormalDraw;
		frame_ = 200;
		m_pPlayer->AnimChangeStandUp();
		return;
	
	}
}
void GameoverScene::FadeOutUpdate(Input&)
{
	frame_--;
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
		//画像解放
		{
			DeleteGraph(m_circleHandle);
			DeleteGraph(m_shadowHandle);
		}

		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}
void GameoverScene::NormalDraw()
{
	DrawRectRotaGraph(CirclePos.x, CirclePos.y,
	kDaenW * 0, kDaenH * 0,//切り取り左上
	kDaenW, kDaenH,//切り取りの幅
	kDaenScale, 0, m_circleHandle, true);


	DrawRectRotaGraph(ShadowPos.x, ShadowPos.y - 20.0f,
	kShadowW * 0, kShadowH * 0,//切り取り左上
	kShadowW, kShadowH,//切り取りの幅
	kShadowScale, 0, m_shadowHandle, true);



	m_pPlayer->Draw(camera);
	if (m_pDroppedItem) m_pDroppedItem->DroppedDraw(camera);
	DrawString(320, 240, "Game Over Scene", 0xffffff);
}
void GameoverScene::FadeInDraw()
{
}
void GameoverScene::FadeOutDraw()
{
}


void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);

}

void GameoverScene::Draw()
{
	
	(this->*draw_)();

}