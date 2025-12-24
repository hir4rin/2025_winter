#include "StageUI.h"
#include "DxLib.h"
#include "Camera.h"
#include <cmath>


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
	//HPバーの右側座標
	const float rightHpX = ScreenWidth * 0.5 * 1/2;
	//hpの表示用の割合
	const float hpScale = rightHpX / 100;


	//変身のデザイン
	const float secondRightX = ScreenWidth * 0.5 * 8 / 5;
	//残機数
	const float thirdRihtX = ScreenWidth - 50;
	
	//BossUI
	constexpr float ktopOffset = 100.0f;



}


StageUI::StageUI() :
	m_playerHp(100),
	m_displayHp(100),
	m_bossHp(100),
	m_displayBossHp(100)
{
	m_hpHandle;
}

StageUI::~StageUI()
{

}



void StageUI::Init(int hp)
{
	m_playerHp = hp;
	m_displayHp = hp;
}
void StageUI::Update()
{
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
	DrawBox(leftHpX, topPos + 50, leftHpX + 100 * hpScale, topPos + 100, GetColor(0, 0, 0), true);
	DrawBox(leftHpX, topPos + 50, leftHpX + m_displayHp * hpScale, topPos + 100, GetColor(255, 170, 80), true);
	DrawBox(leftHpX, topPos + 50, leftHpX + 100 * hpScale, topPos + 100, GetColor(255, 0, 0), false);//外枠
	DrawFormatString(300, topPos + 60, GetColor(0, 0, 0), "Player : ");

	//変身のデザイン
	DrawBox(firstRightX + 50, topPos + 30, secondRightX, bottomUI, col, true);//幅200
	CopyDesign();
	//残機数
	DrawBox(secondRightX + 50, topPos + 80, thirdRihtX, bottomUI, col, true);//幅120
	DrawFormatString(secondRightX + 150, topPos + 100, GetColor(0, 0, 0), "%d", m_playerLife);



	if (camera.isBoss)
	{
		//敵のHPバーを表示
		DrawBox(leftHpX, topPos + 50+ ktopOffset, leftHpX + 100 * hpScale, topPos + 100 + ktopOffset, GetColor(0, 0, 0), true);
		DrawBox(leftHpX, topPos + 50 + ktopOffset, leftHpX + m_displayBossHp * hpScale, topPos + 100 + ktopOffset, GetColor(255, 170, 80), true);
		DrawBox(leftHpX, topPos + 50 + ktopOffset, leftHpX + 100 * hpScale, topPos + 100 + ktopOffset, GetColor(255, 0, 0), false);//外枠
		DrawFormatString(300, topPos + 60 + ktopOffset, GetColor(0, 0, 0), "Boss : ");
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
}

void StageUI::SetLife(int m_pPlayerLife)
{
	m_playerLife = m_pPlayerLife;
}

void StageUI::SetBossHp(int m_pBossHp)
{
	m_bossHp = m_pBossHp;
}

float StageUI::GetTopX()
{
	return topPos;
}

void StageUI::CopyDesign()
{
	switch (m_displayType)
	{
	case PlayerType::Normal:
		DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Normal");
		break;
	case PlayerType::Burning:
		DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Burning");
		break;
	case PlayerType::Frozen:
		DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Frozen");
		break;
	case PlayerType::Archer:
		DrawFormatString(firstRightX+ 150, topPos + 100, GetColor(0, 0, 0), "Archer");
		break;
	}
}
