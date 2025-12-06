#include "GameClearScene.h"
#include "DxLib.h"
#include "../Input.h"
#include "Cannon.h"
#include "TitleScene.h"
#include "SceneController.h"
#include "Bg.h"




GameClearScene::GameClearScene(SceneController& controller) : Scene(controller)
{
	m_pCannon = std::make_shared<Cannon>();
	m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100);
	m_pBg = new Bg(m_pPlayer, 1);

	m_pPlayer->SetBgPointer(m_pBg);
	InitCamera(camera);
}

void GameClearScene::Update(Input& input)
{
	UpdateCamera(camera, m_pPlayer);
	

	m_pCannon->Update();
	//お試し--------------------------------------
	if (!isCannon)m_pPlayer->Update(input);

	m_pPlayer->AutoMove();
	//---------------------------------------------L

	if (input.IsTriggered("ok"))
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}

	//大砲に乗ったら。。の処理
	if (!isCannon)
	{
		if (m_pPlayer->GetColRect().IsCollision(m_pCannon->GetColRect()))
		{
			//m_pPlayer = nullptr;
			m_pPlayer->ChangeVel() += Vec2{ 60.0f,-20.0f};//右上に飛ばす
			isCannon = true;
		}
	}
	//デバッグ用
	if (input.IsTriggered("Jump"))
	{
		isCannon = false;
	}
	

}

void GameClearScene::Draw()
{
	m_pBg->Draw(camera);

	DrawString(320, 240, "Game Clear Scene", 0xffffff);


	m_pPlayer->Draw(camera);
	m_pCannon->Draw(camera);
}