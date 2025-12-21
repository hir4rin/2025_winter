#include "TitleLightEffect.h"
#include "DxLib.h"


namespace
{
	//light
	constexpr int kLightCutW = 128;
	constexpr int kLightCutH = 128;
	constexpr float kLightScale = 2.0f;

	constexpr int kDuration = 6;
	constexpr int kNum = 5;

	//下の丸
	constexpr int kDaenW = 64;
	constexpr int kDaenH = 64;
	constexpr float kDaenScale = 2.0f;
	//その影
	constexpr int kShadowW = 100;
	constexpr int kShadowH = 100;
	constexpr float kShadowScale = 3.0f;
}






TitleLightEffect::TitleLightEffect(Vec2 pos,int num):
	charaIdx(0),
	charaIdy(0),
	m_animFrame(0),
	m_isDead(false)
{ 
	if (num == 1)
	{
	handleNum = 1;
	m_pos = pos;
	m_handle = LoadGraph("data/LightEffect.png");
	}

	if (num == 2)
	{
		handleNum = 2;
		m_pos = pos;
		m_handle = LoadGraph("data/daen.png");
	}
	if (num == 3)
	{
		handleNum = 3;
		m_pos = pos;
		m_handle = LoadGraph("data/Game/Shadow.png");
	}
}

TitleLightEffect::~TitleLightEffect()
{
	DeleteGraph(m_handle);
}

void TitleLightEffect::Update()
{
	m_animFrame++;

	if (handleNum == 1)
	{
		charaIdx = m_animFrame / kDuration % kNum;
		charaIdy = m_animFrame / kDuration / kNum % 3;

		if (charaIdx == kNum - 1 && charaIdy == 2)//最後まで行ったら
		{
			m_isDead = true;
		}
	}

	if (handleNum == 2 || handleNum == 3)
	{
		if (m_animFrame > kDuration * 9)//ライトが9番目の画像だったら
		{
			m_isDead = true;
		}
	}

}
void TitleLightEffect::Draw()
{
	if (handleNum == 1)
	{
	DrawRectRotaGraph(m_pos.x, m_pos.y,
	kLightCutW * charaIdx, kLightCutH * charaIdy,//切り取り左上
	kLightCutW, kLightCutH,//切り取りの幅
	kLightScale, 180.0f * DX_PI / 180.0f, m_handle, true);
	}

	
	if (handleNum == 2)
	{
		if (charaIdy <= 1)
		{
			DrawRectRotaGraph(m_pos.x, m_pos.y,
		kDaenW * 0, kDaenH * 0,//切り取り左上
		kDaenW, kDaenH,//切り取りの幅
		kDaenScale, 0, m_handle, true);
		}
	}
	if (handleNum == 3)
	{
		if (charaIdy <= 1)
		{
			DrawRectRotaGraph(m_pos.x, m_pos.y - 20.0f,
		kShadowW * 0, kShadowH * 0,//切り取り左上
		kShadowW, kShadowH,//切り取りの幅
		kShadowScale, 0, m_handle, true);
		}
	}
	


	
}
