#include "StageUI.h"
#include "DxLib.h"
#include "Scene/SceneController.h"
#include "Camera.h"
#include <cmath>
#include <string>


namespace
{
	constexpr int ScreenWidth = 1920;
	constexpr int ScreenHeight = 1080;

	const float topPos = ScreenHeight * 0.75;//カメラにも置いた
	//UIの下部分
	const float bottomUI = ScreenHeight - 50;

	//HP,SCOREバーの下地の右側の座標
	const float firstRightX = ScreenWidth * 0.5 * 6 / 5;
	//HPバーの左側座標
	const float leftHpX = ScreenWidth * 0.5 * 3 / 5;
	//HPバーの右側座標(左側座標からどれくらい伸ばすか)
	const float rightHpX = ScreenWidth * 0.5 * 24 / 100;
	//hpの表示用の割合
	const float hpScale = rightHpX / 100;


	//変身のデザイン
	const float secondRightX = ScreenWidth * 0.5 * 8 / 5;
	//残機数
	const float thirdRihtX = ScreenWidth - 50;

	//BossUI
	constexpr float ktopOffset = 100.0f;

	//Hpバーのサイズ
	constexpr int kHpBarW = 812;
	constexpr int kHpBarH = 161;
	constexpr float kHpBarScale = 0.4f;
	//BossHpバーのサイズ
	constexpr int kHpBossBarW = 840;
	constexpr int kHpBossBarH = 158;
	constexpr float kHpBossBarScale = 0.40f;

	//コピーデザインのUI
	constexpr int kNormalCutW = 848;
	constexpr int kNormalCutH = 1264;
	constexpr int kFrozenCutW = 798;
	constexpr int kFrozenCutH = 864;
	constexpr int burning_cut_w = 992;
	constexpr int burning_cut_h = 1072;
	constexpr int arrow_cut_w = 848;
	constexpr int arrow_cut_h = 1264;

	constexpr float kNormalScale = 0.14f;
	constexpr float  kFrozenScale = 0.20f;
	constexpr float  burning_scale = 0.17f;
	constexpr float  arrow_scale = 0.14f;


	//キャラクターのサイズ
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ//当たり判定の幅高さに使われている
	constexpr int kPlayerCutW = 100;
	constexpr int kPlayerCutH = 100;
	constexpr float  kPlayerScale = 3.0f;


}


StageUI::StageUI(SceneController& controller) :
	m_frame(0),
	m_playerHp(100),
	m_displayHp(100),
	m_bossHp(100),
	m_displayBossHp(100),
	m_bossFishHp(100),
	m_displayBossFishHp(100),
	m_BearWolfHp(100),
	m_displayBearWolfhp(100),
	m_fontHandle(-1),
	m_pPlayer(std::make_shared<Player>(PlayerType::Normal, 100, Vec2(secondRightX + 100, topPos + 100),2, controller.GetEffekseerResourceManager()))
{
	m_hpHandle = LoadGraph("data/Game/HpBar.png");
	m_bossHpHandle = LoadGraph("data/Game/HpBossBar.png");
	m_normalHandle = LoadGraph("data/Game/SwordCard.png");
	m_burningHandle = LoadGraph("data/Game/BurningCardRE.png");
	m_frozenHandle = LoadGraph("data/Game/FrozenCard.png");
	m_archerHandle = LoadGraph("data/Game/ArrowCardRE.png");
	//m_pPlayer = std::make_shared<Player>(PlayerType::Normal, 100, Vec2(secondRightX + 100, topPos + 100), 3);
	//フォントの生成
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 48, 6, -1);
}

StageUI::~StageUI()
{
	//生成したフォントの削除
	DeleteFontToHandle(m_fontHandle);
}



void StageUI::Init(int hp, PlayerType type, int Life)
{
	//hp
	m_playerHp = hp;
	m_displayHp = hp;
	//playerType
	m_displayType = type;
	switch (type)
	{
	case PlayerType::Normal:
		m_pPlayer->ChangeNormal();
		break;
	case PlayerType::Burning:
		m_pPlayer->ChangeBurning();
		break;
	case PlayerType::Frozen:
		m_pPlayer->ChangeFrozen();
		break;
	case PlayerType::Archer:
		m_pPlayer->ChangeArcher();
		break;
	}
	//残機
	m_playerLife = Life;


}
void StageUI::Update()
{
	m_frame++;
	const float t = 0.1f;//小さい程ゆっくりになる
	if (m_playerHp == 0)//HPがゼロの時はlerpしない
	{
		m_displayHp = 0;
	}
	else//ゼロ以外はラープ
	{
		m_displayHp = std::lerp(m_displayHp, static_cast<float>(m_playerHp), t);
	}

	m_displayBossHp = std::lerp(m_displayBossHp, static_cast<float>(m_bossHp), t);
	m_displayBossFishHp = std::lerp(m_displayBossFishHp, static_cast<float>(m_bossFishHp), t);
	m_displayBearWolfhp = std::lerp(m_displayBearWolfhp, static_cast<float>(m_BearWolfHp), t);

	if (m_frame % 2 == 0)m_pPlayer->AnimFrameUpdate();
	m_pPlayer->AnimOnlyUpdate();

}

void StageUI::Draw(Camera& camera)
{
	//フォントの大きさを変える
	int oldSize = GetFontSize();
	SetFontSize(32);
	SetFontThickness(100);//フォントの太さ
	int col = GetColor(252, 252, 168);

	//UIの下のバーの部分
	DrawBox(0, topPos, ScreenWidth, ScreenHeight, GetColor(255, 170, 80), true);
	//HP,SCOREバーの下地
	DrawBox(50, topPos + 20, firstRightX, bottomUI, col, true);

	//HPバー
	DrawBox(leftHpX, topPos + 62, leftHpX + 100 * hpScale, topPos + 105, GetColor(0, 0, 0), true);//下地の黒部分
	DrawBox(leftHpX, topPos + 62, leftHpX + m_displayHp * hpScale, topPos + 105, GetColor(255, 170, 80), true);//hpバー
	//DrawBox(leftHpX, topPos + 50, leftHpX + 100 * hpScale, topPos + 100, GetColor(255, 0, 0), false);//外枠
	DrawRectRotaGraph(leftHpX + 79, topPos + 79,
		kHpBarW * 0, kHpBarH * 0,//切り取り左上
		kHpBarW, kHpBarH,//切り取りの幅
		kHpBarScale, 0, m_hpHandle, true);
	DrawStringToHandle(360, topPos + 60, " Player : ", GetColor(0, 0, 0), m_fontHandle);

	//変身のデザイン
	DrawBox(firstRightX + 50, topPos + 20, secondRightX, bottomUI, col, true);//幅200
	CopyDesign();
	//残機数
	DrawBox(secondRightX + 50, topPos + 80, thirdRihtX, bottomUI, col, true);//幅120
	DrawRectRotaGraph(secondRightX + 100, topPos + 170,
		kPlayerCutW * m_pPlayer->GetAnimIdx(), kPlayerCutH * 1,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, 0, m_pPlayer->GetHandle(), true);

	std::string text = "× " + std::to_string(m_playerLife);//残機数表示用テキスト
	DrawStringToHandle(secondRightX + 160, topPos + 140, text.c_str(), GetColor(0, 0, 0), m_fontHandle);

	if (camera.isBoss)
	{

		//敵のHPバーを表示
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + 100 * hpScale * 1.3f, topPos + 92 + ktopOffset, GetColor(0, 0, 0), true);
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + m_displayBossHp * hpScale*1.3f, topPos + 92 + ktopOffset, GetColor(80, 200, 255), true);
		//DrawBox(leftHpX, topPos + 50 + ktopOffset, leftHpX + 100 * hpScale, topPos + 100 + ktopOffset, GetColor(255, 0, 0), false);//外枠
		DrawRectRotaGraph(leftHpX + 135, topPos + 50 + ktopOffset + 25,
			kHpBossBarW * 0, kHpBossBarH * 0,//切り取り左上
			kHpBossBarW, kHpBossBarH,//切り取りの幅
			kHpBossBarScale, 0, m_bossHpHandle, true);
		DrawStringToHandle(360, topPos + 57 + ktopOffset, "Boss : ", GetColor(0, 0, 0), m_fontHandle);
	}
	if (camera.isFish)
	{
		//敵のHPバーを表示
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + 100 * hpScale * 1.3f, topPos + 92 + ktopOffset, GetColor(0, 0, 0), true);
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + m_displayBossFishHp * hpScale * 1.3f, topPos + 92 + ktopOffset, GetColor(80, 200, 255), true);
		//DrawBox(leftHpX, topPos + 50 + ktopOffset, leftHpX + 100 * hpScale, topPos + 100 + ktopOffset, GetColor(255, 0, 0), false);//外枠
		DrawRectRotaGraph(leftHpX + 135, topPos + 50 + ktopOffset + 25,
			kHpBossBarW * 0, kHpBossBarH * 0,//切り取り左上
			kHpBossBarW, kHpBossBarH,//切り取りの幅
			kHpBossBarScale, 0, m_bossHpHandle, true);
		DrawStringToHandle(360, topPos + 57 + ktopOffset, "Fishers : ", GetColor(0, 0, 0), m_fontHandle);
	}
	if (camera.isBearWolf)
	{
		//敵のHPバーを表示
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + 100 * hpScale * 1.3f, topPos + 92 + ktopOffset, GetColor(0, 0, 0), true);
		DrawBox(leftHpX, topPos + 55 + ktopOffset, leftHpX + m_displayBearWolfhp * hpScale * 0.5f * 1.3f, topPos + 92 + ktopOffset, GetColor(80, 200, 255), true);
		//DrawBox(leftHpX, topPos + 50 + ktopOffset, leftHpX + 100 * hpScale, topPos + 100 + ktopOffset, GetColor(255, 0, 0), false);//外枠
		DrawRectRotaGraph(leftHpX + 135, topPos + 50 + ktopOffset + 25,
			kHpBossBarW * 0, kHpBossBarH * 0,//切り取り左上
			kHpBossBarW, kHpBossBarH,//切り取りの幅
			kHpBossBarScale, 0, m_bossHpHandle, true);
		DrawStringToHandle(280, topPos + 57 + ktopOffset, "Bear&Wolf : ", GetColor(0, 0, 0), m_fontHandle);
	}

	//フォントサイズを元に戻す(元は16)
	SetFontSize(oldSize);
	SetFontThickness(1);//フォントの太さ

}

void StageUI::SetHp(int  m_pPlayerHP)
{
	m_playerHp = m_pPlayerHP;
	if (m_playerHp < 0)//0を下回ったら0にする
	{
		m_playerHp = 0;
	}
}

void StageUI::SetType(PlayerType m_type)
{
	m_displayType = m_type;

	if (m_pPlayer->GetType() == m_type)//同じ変身なら変更しない
	{
		return;
	}
	switch (m_type)
	{
	case PlayerType::Normal:
		m_pPlayer->ChangeNormal();
		break;
	case PlayerType::Burning:
		m_pPlayer->ChangeBurning();
		break;
	case PlayerType::Frozen:
		m_pPlayer->ChangeFrozen();
		break;
	case PlayerType::Archer:
		m_pPlayer->ChangeArcher();
		break;
	}
}

void StageUI::SetLife(int m_pPlayerLife)
{
	m_playerLife = m_pPlayerLife;
}

void StageUI::SetBossHp(int m_pBossHp)
{
	m_bossHp = m_pBossHp;
}

void StageUI::SetFishHp(int m_pBossFishHp)
{
	m_bossFishHp = m_pBossFishHp;
}

void StageUI::SetBearWolfHp(int bearHp, int WolfHp)
{
	m_BearWolfHp = bearHp + WolfHp;
}

float StageUI::GetTopX()
{
	return topPos;
}

void StageUI::CopyDesign()
{

	Vec2 UIPos = { firstRightX + 200,topPos + 120 };
	switch (m_displayType)
	{
	case PlayerType::Normal:
		//DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Normal");
		DrawRectRotaGraph(UIPos.x, UIPos.y,
			kNormalCutW * 0, kNormalCutH * 0,//切り取り左上
			kNormalCutW, kNormalCutH,//切り取りの幅
			kNormalScale, 0.0f, m_normalHandle, true);
		break;
	case PlayerType::Burning:
		//DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Burning");
		DrawRectRotaGraph(UIPos.x, UIPos.y,
			burning_cut_w * 0, burning_cut_h * 0,//切り取り左上
			burning_cut_w, burning_cut_h,//切り取りの幅
			burning_scale, 0.0f, m_burningHandle, true);
		break;
	case PlayerType::Frozen:
		//DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Frozen");
		DrawRectRotaGraph(UIPos.x, UIPos.y,
			 kFrozenCutW * 0, kFrozenCutH * 0,//切り取り左上
			 kFrozenCutW, kFrozenCutH,//切り取りの幅
			 kFrozenScale, 0.0f, m_frozenHandle, true);
		break;
	case PlayerType::Archer:
		//DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Archer");
		DrawRectRotaGraph(UIPos.x, UIPos.y,
			arrow_cut_w * 0, arrow_cut_h * 0,//切り取り左上
			arrow_cut_w, arrow_cut_h,//切り取りの幅
			arrow_scale, 0.0f, m_archerHandle, true);
		break;
	}
}
