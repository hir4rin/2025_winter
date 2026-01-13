#include "GameClearScene.h"
#include "Application.h"
#include "DxLib.h"
#include "../Input.h"
#include "Cannon.h"
#include "Sign.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "SceneController.h"
#include "Bg.h"
#include "Player.h"
#include "Effect.h"
#include <cmath>
#include <cassert>

namespace
{
	constexpr float kPower = 1.0f;//大砲の威力
	constexpr float kSpeed = 1.7f;//ゲージのスピード

	constexpr int margin = 100;//一旦マージンを取る
	constexpr int kRankWidth = 1500;//位差の幅

	constexpr float kSlowDownDistance = 1000.0f; // 減速開始距離
	constexpr float kGravity = 2.5f; // 重力

	constexpr int fade_interval = 60;

}


GameClearScene::GameClearScene(SceneController& controller,PlayerType type,int hp,int Life) : Scene(controller),
update_(&GameClearScene::FadeInUpdate),
draw_(&GameClearScene::FadeDraw)
{

	m_pCannon = std::make_shared<Cannon>();
	for (int i = 0; i < 7; i++)
	{
		m_pSigns.push_back(std::make_shared<Sign>(Vec2{ kRankWidth * (i+1)- 100.0f,764.0f},7-i));
	}
	m_pPlayer = std::make_shared<Player>(type, hp, Vec2{ 100,800 },Life);
	m_pBg = new Bg(m_pPlayer, 4);
	m_frame = fade_interval;// フェードインの最初
	m_pPlayer->SetBgPointer(m_pBg);
	InitCamera(camera,4);//カメラの初期化

	//シーン切り替え後のにゅいーんをなくす
	stageUI.Init(hp, m_pPlayer->GetType(), m_pPlayer->GetLife());


	m_pPlayer->AddOnWalkEvent([this]() {
		m_pEffects.push_back(std::make_shared<Effect>(m_pPlayer->GetPos(), "dustForClear",false));
			});
}

void GameClearScene::FadeInUpdate(Input&)
{
	m_pPlayer->AnimFrameUpdate();
	if (m_pPlayer != nullptr)UpdateCamera(camera, m_pPlayer);
	if (m_frame-- <= 0)
	{

		update_ = &GameClearScene::NormalUpdate;
		draw_ = &GameClearScene::NormalDraw;
		return;
	}
}
void GameClearScene::NormalUpdate(Input& input)
{
	//ゲージの更新
	//60フレームに1秒
	m_timer += 1.0f / 60, .0f;
	m_gaugeTimer = fmod(m_timer * kSpeed, 1.0f);//0~1秒の間ループ

	float tri;//triangle waveの略(三角波)
	if (m_gaugeTimer < 0.5f)
	{
		tri = m_gaugeTimer * 2.0f;//0~1
	}
	else
	{
		tri = (1.0f - m_gaugeTimer) * 2.0f;//1~0
	}

	if (!isFlying)
	{
		m_gaugeCursorX = std::lerp(m_gaugeleftX, m_gaugerightX, tri);
	}
	//目押し処理終了------------------------------
	//1位から7位までを決める
	if (m_finalTri < 1 / 7.0f)m_rank = 7;//7位
	else if (m_finalTri < 2 / 7.0f)m_rank = 6;//6位
	else if (m_finalTri < 3 / 7.0f)m_rank = 5;//5位
	else if (m_finalTri < 4 / 7.0f)m_rank = 4;//4位
	else if (m_finalTri < 5 / 7.0f)m_rank = 3;//3位
	else if (m_finalTri < 6 / 7.0f)m_rank = 2;//2位
	else   m_rank = 1;//6/7.0f~1.0f//1位


	if (m_pPlayer != nullptr)UpdateCamera(camera, m_pPlayer);


	m_pCannon->Update();
	for (auto& s : m_pSigns)
	{
		s->Update();
	}
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
	//お試し--------------------------------------
	//if (!isCannon && m_pPlayer != nullptr)m_pPlayer->Update(input);
	if ((!isCannon && !isFlying) &&  m_pPlayer != nullptr)
	{
		m_pPlayer->AnimFrameUpdate();
	
		m_pPlayer->ChangeVel().x = 6.0f;//自動で右に進む//ジャンプするようにしたい
		m_pPlayer->AutoMoveStart();
	}

	if (m_pPlayer != nullptr)
	{
		if (isFlying)m_pPlayer->AutoMove();
	}
	//---------------------------------------------

	if (input.IsTriggered("ok") )
	{
		controller_.ChangeScene(std::make_shared<StageSelectScene>(controller_,m_pPlayer->GetType(),m_pPlayer->GetHp(),m_pPlayer->GetLife()));
		return;
	}

	//大砲に乗ったら。。の処理
	if (!isCannon && !isFlying)
	{
		if (m_pPlayer->GetColRect().IsCollision(m_pCannon->GetColRect()))
		{
			//大砲に乗るアニメーション
			m_pCannon->CannonTransForm();
			//m_pPlayer = nullptr;
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };//速度を0にする

			isCannon = true;
		}
	}
	//大砲から発射する処理
	if (isCannon)
	{
		if (input.IsTriggered("Jump") ||input.IsTriggered("Attack"))
		{
			isFlying = true;
			isCannon = false;
			//ここで大砲から発射するアニメーション
			m_pCannon->CannonFire();
			//大砲からプレイヤーを発射
		/*	m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100);
			m_pPlayer->SetBgPointer(m_pBg);*/
			m_pPlayer->ChangePos() = m_pCannon->GetPos();//大砲の先端に座標を合わせる//+ Vec2{ 50.0f,0.0f }
			m_pPlayer->ChangeVel() = Vec2{ 30.0f * kPower,0.0f };//右上に飛ばす
			m_pPlayer->AnimSelect(Anim::Jump);

			//順位づけの処理
			m_finalTri = tri;
		}
	}
	if (isFlying)//順位によって止まる位置を変える
	{
		//減速処理
		ToArrivedAtGoal();
		m_pPlayer->AnimFrameUpdate();
		//大砲から飛び出した後の処理
		//isFlying = false;
	}
	//エフェクトのUpdate
	for (auto effect = m_pEffects.begin(); effect != m_pEffects.end(); )
	{
		(*effect)->Update();

		if ((*effect)->IsDead())
			effect = m_pEffects.erase(effect);  // ← 安全に削除
		else
			++effect;
	}
	if (isArrived)
			{
		update_ = &GameClearScene::FadeOutUpdate;
		draw_ = &GameClearScene::FadeDraw;
		m_frame = 0;
		return;
	}

	


}
void GameClearScene::FadeOutUpdate(Input&)
{
	//一旦のエリア
	{
	//減速処理
	ToArrivedAtGoal();
	m_pPlayer->AnimFrameUpdate();
	m_pPlayer->AutoMove();

	//エフェクトのUpdate
	for (auto effect = m_pEffects.begin(); effect != m_pEffects.end(); )
	{
		(*effect)->Update();

		if ((*effect)->IsDead())
			effect = m_pEffects.erase(effect);  // ← 安全に削除
		else
			++effect;
	}

	}

	//NormalDraw();
	if (m_frame++ >= fade_interval * 5)
	{
		//delete m_pCharacter;
		delete m_pBg;
		controller_.ChangeScene(std::make_shared<StageSelectScene>(controller_,m_pPlayer->GetType(),m_pPlayer->GetHp(),m_pPlayer->GetLife()));
		return;
	}
}
void GameClearScene::FadeDraw()
{
	//normal draw
	NormalDraw();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	float rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	//DrawBox(0, 0, screenWidth, screenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}
void GameClearScene::NormalDraw()
{
	m_pBg->Draw(camera);
	//ステージUI
	stageUI.Draw(camera);
#ifdef _DEBUG
	DrawFormatString(500, 300, GetColor(255, 0, 0), "Your Rank is %d", m_rank);
	//白線
	for (int i = 1; i <= 7; i++)
	{
		DrawBox(kRankWidth * i + camera.drawOffset.x,//左
			0 + camera.drawOffset.y,//下
			kRankWidth * i + camera.drawOffset.x,//右
			1080 + camera.drawOffset.y,//上
			GetColor(255, 255, 255), false);
	}
#endif

	//大砲のゲージ表示
	if (isCannon ||isFlying)
	{
		//ゲージの中
		DrawBox(m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugeleftX,//左
	m_pCannon->GetPos().y + camera.drawOffset.y - margin,//下
	m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugerightX,//右
		m_pCannon->GetPos().y + camera.drawOffset.y - margin + 50,//上
	GetColor(0, 0, 0), true);


		//実際のゲージ
		DrawBox(m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugeleftX,//左
		m_pCannon->GetPos().y + camera.drawOffset.y - margin,//下
		m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugeCursorX,//右
			m_pCannon->GetPos().y + camera.drawOffset.y - margin + 50,//上
		GetColor(0, 255, 0), true);
		//ゲージの枠
		DrawBox(m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugeleftX,//左
		m_pCannon->GetPos().y + camera.drawOffset.y - margin,//下
		m_pCannon->GetPos().x + camera.drawOffset.x + margin + m_gaugerightX,//右
			m_pCannon->GetPos().y + camera.drawOffset.y - margin + 50,//上
		GetColor(255, 255, 0), false);

	}


	if (!isCannon || isFlying)m_pPlayer->Draw(camera);
	m_pCannon->Draw(camera);

	for(auto& s: m_pSigns)
	{
		s->Draw(camera);
	}

	for (auto& effect : m_pEffects)
	{
		if (effect) effect->Draw(camera);
	}

}

void GameClearScene::ToArrivedAtGoal()
{
	//到達目標地点
	float targetX = kRankWidth * (8 - m_rank); // 1位=7, 2位=6

	switch (m_rank)
	{
	case 0:
		break;
	case 1://1位
		
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			//全体の進捗度合い
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			//速度の割合(2乗を使って滑らか) y=1-x*x
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに(イージング関数というやつらしい)(ググればわかる
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 2://2位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 3://3位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 4://4位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 5://5位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 6://6位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	case 7://7位
		//減速開始判定
		if (!isSlowDown && m_pPlayer->GetPos().x > targetX - kSlowDownDistance)
		{
			//目標地点までの距離に応じて速度を減速
			isSlowDown = true;
			m_slowDownStartX = m_pPlayer->GetPos().x;
			m_pPlayer->ChangeVel().y += kGravity;//下に落ちるように
		}
		//減速
		if (isSlowDown)
		{
			//減速処理
			float progress = (m_pPlayer->GetPos().x - m_slowDownStartX) / kSlowDownDistance;
			float speedRate = 1.0f - progress * progress;//二乗で減速を滑らかに
			m_pPlayer->ChangeVel().x = 30.0f * kPower * speedRate;
		}
		//停止判定
		if (m_pPlayer->GetPos().x >= targetX)
		{
			m_pPlayer->ChangeVel() = Vec2{ 0.0f,0.0f };
			isArrived = true;
		}
		break;
	default:
		assert("Error:Rank out of range");
		break;
	}

	if (!isSlowDown && isFlying)
	{
		//いったん回転させる
		m_pPlayer->RotateUpdate();
	}
	if (isSlowDown)
	{
		m_pPlayer->RotateFinishUpdate();
	}
}

void GameClearScene::Update(Input& input)
{
	(this->*update_)(input);
}
void GameClearScene::Draw()
{
	(this->*draw_)();
}
