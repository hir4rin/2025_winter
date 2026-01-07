#include "StageSelectScene.h"
#include "DxLib.h"
#include "Player.h"
#include "../input.h"
#include "GameScene.h"
#include "Bg.h"
#include "Door.h"
#include "Effect.h"
#include "SceneController.h"
#include "../Application.h"




namespace
{
	constexpr int fade_interval = 60;
	constexpr int selectMax = 1;//最大-1

	constexpr int margin = 200;
}


void StageSelectScene::FadeInUpdate(Input&)
{

	m_pPlayer->AnimFrameUpdate();
	if (m_pPlayer != nullptr)UpdateCamera(camera, m_pPlayer);




	if (m_frame-- <= 0)
	{

		update_ = &StageSelectScene::NormalUpdate;
		draw_ = &StageSelectScene::NormalDraw;
		return;
	}

}

void StageSelectScene::NormalUpdate(Input& input)
{
	//カメラ
	UpdateCamera(camera, m_pPlayer);

	//背景
	m_pBg->Update();
	//ステージUI
	{
		//プレイヤーのHPを引き渡す
		stageUI.SetHp(m_pPlayer->GetHp());
		//プレイヤーのTypeを引き渡す
		stageUI.SetType(m_pPlayer->GetType());
		///プレイヤーのlifeを引き渡す
		stageUI.SetLife(m_pPlayer->GetLife());

		//UIのアップデート
		stageUI.Update();
	}
	for (auto& door : m_doors)
	{
		door->Update();
	}

	m_pPlayer->Update(input);
	//エフェクト
	for (auto effect = m_pEffects.begin(); effect != m_pEffects.end(); )
	{
		(*effect)->Update();

		if ((*effect)->IsDead())
			effect = m_pEffects.erase(effect);  // ← 安全に削除
		else
			++effect;
	}


	if (input.IsTriggered("up"))
	{
		//selectIndexを上げる
		selectIndex--;
		if (selectIndex < 0)
		{
			selectIndex = 0;
		}
	}
	if (input.IsTriggered("down"))
	{
		//selectIndexを下げる
		selectIndex++;
		if (selectIndex > selectMax)
		{
			selectIndex = selectMax;
		}
	}

	//ドアに触れているかつ上入力をしていたらシーン遷移
	for (auto& door : m_doors)
	{
		if (m_pPlayer->GetColRect().IsCollision(door->GetColRect()) && input.IsTriggered("up"))
		{

			update_ = &StageSelectScene::FadeOutUpdate;
			draw_ = &StageSelectScene::FadeOutDraw;
			m_frame = 0;

		}
	}
	

	//if (input.IsTriggered("Jump"))//決定
	//{
	//	if (selectIndex+1 == 1)//ステージ1
	//	{
	//		controller_.ChangeScene(std::make_shared<GameScene>(controller_, 1, PlayerType::Normal, 100,5));
	//		return;
	//	}
	//	if (selectIndex+1 == 2)//ステージ2
	//	{
	//		controller_.ChangeScene(std::make_shared<GameScene>(controller_, 6, PlayerType::Normal, 100,5));
	//		return;
	//	}

	//}
}

void StageSelectScene::FadeOutUpdate(Input&)
{
	//playerに触れているドアだけのアップデート
	for (auto& door : m_doors)
	{
		if (m_pPlayer->GetColRect().IsCollision(door->GetColRect()))
		{
			door->OutUpdate();
		}
	}

	if (m_frame++ >= fade_interval)
	{
		//delete m_pCharacter;
		delete m_pBg;
		//ドアに触れているドアIDでのシーン遷移
		for (auto& door : m_doors)
		{
			if (m_pPlayer->GetColRect().IsCollision(door->GetColRect()))
			{
				//1_1へ
				if (door->GetDoorID() == StageID::Fstage)
				{
					controller_.ChangeScene(std::make_shared<GameScene>(controller_, 1,m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
					return;
				}
				//2_1へ
				if (door->GetDoorID() == StageID::Sstage)
				{
					controller_.ChangeScene(std::make_shared<GameScene>(controller_, 6, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
					return;
				}
				//3_1へ
				if (door->GetDoorID() == StageID::Tstage)
				{
					controller_.ChangeScene(std::make_shared<GameScene>(controller_, 9, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
					return;
				}
				//4_1へ
				if (door->GetDoorID() == StageID::Forthstage)
				{
					controller_.ChangeScene(std::make_shared<GameScene>(controller_, 10, m_pPlayer->GetType(), m_pPlayer->GetHp(), m_pPlayer->GetLife()));
					return;
				}


			}
		}


	}
}

void StageSelectScene::NormalDraw()
{

	m_pBg->Draw(camera);
	
	for (auto& door : m_doors)
	{
		door->Draw(camera);
	}
	m_pPlayer->Draw(camera);
	for (auto& effect : m_pEffects)
	{
		if (effect) effect->Draw(camera);
	}

	//ステージUI
	stageUI.Draw(camera);
	const auto& wsize = Application::GetInstance().GetWindowSize();

	/*DrawFormatString(wsize.w / 2 - margin, wsize.h/2 + (selectIndex) * margin, GetColor(0, 0, 0), "⇒");
	DrawFormatString(wsize.w / 2, wsize.h/2, GetColor(0, 0, 0), "stage_1");
	DrawFormatString(wsize.w / 2, wsize.h/2 + margin, GetColor(0, 0, 0), "stage_2");*/

	
	
}

void StageSelectScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	//// 値の範囲をいったん0.0~1.0にしておくといろいろと扱いやすくなります
	auto rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate); // αブレンド
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);	// 画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドしない
}
void StageSelectScene::FadeInDraw()
{
	NormalDraw();


	//フェード
	m_pBg->FadeInBg(camera);

	
}

void StageSelectScene::FadeOutDraw()
{
	//フェードアウト用の処理
	NormalDraw();

	//フェード
	m_pBg->FadeOutBg(camera);

	

}

StageSelectScene::StageSelectScene(SceneController& controller,PlayerType type,int hp,int Life) : Scene(controller)
{
	
	update_ = &StageSelectScene::FadeInUpdate;
	draw_ = &StageSelectScene::FadeInDraw;

	m_frame = fade_interval;
	m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,736 }, Life);
	//playerの状態によってエフェクトを出す
	m_pPlayer->AddOnLandEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "star"));
		});
	m_pPlayer->AddOnWalkEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "dust"));
		});
	m_pPlayer->AddOnDashEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "dust"));
		});

	
	m_pBg = new Bg(m_pPlayer, 1);

	//シーン切り替え後のにゅいーんをなくす
	stageUI.Init(hp, m_pPlayer->GetType(), m_pPlayer->GetLife());

	m_pPlayer->SetBgPointer(m_pBg);

	InitCamera(camera);//カメラの初期化

	m_doors.push_back(  std::make_shared< Door>(Vec2{ 500,736 },StageID::Fstage));
	m_doors.push_back(  std::make_shared< Door>(Vec2{ 1000,736 },StageID::Sstage));
	m_doors.push_back(  std::make_shared< Door>(Vec2{ 1600,736 },StageID::Tstage));
	m_doors.push_back(  std::make_shared< Door>(Vec2{ 2200,736 },StageID::Forthstage));

}

void StageSelectScene::Update(Input& input)
{
	(this->*update_)(input);
}

void StageSelectScene::Draw()
{
	(this->*draw_)();
}