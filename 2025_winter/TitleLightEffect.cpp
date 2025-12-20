#include "TitleLightEffect.h"
#include "DxLib.h"


namespace
{
	//light
	constexpr int kLightCutW = 128;
	constexpr int kLightCutH = 128;
	constexpr float kLightScale = 2.0f;

	constexpr int kDuration = 10;
	constexpr int kNum = 5;

	//下の丸
	constexpr int kDaenW = 64;
	constexpr int kDaenH = 64;
	constexpr float kDaenScale = 2.0f;
}






TitleLightEffect::TitleLightEffect(Vec2 pos):
	charaIdx(0),
	charaIdy(0),
	m_animFrame(0),
	m_isDead(false)
{ 
	m_pos = pos;
	m_handle = LoadGraph("data/LightEffect.png");
	m_handleDaen = LoadGraph("data/daen.png");
}

TitleLightEffect::~TitleLightEffect()
{
	DeleteGraph(m_handle);
}

void TitleLightEffect::Update()
{
	m_animFrame++;

	charaIdx = m_animFrame / kDuration % kNum;
	charaIdy = m_animFrame / kDuration / kNum % 3;

	if (charaIdx == kNum - 1 && charaIdy == 2)//最後まで行ったら
	{
		m_isDead = true;
	}
}
void TitleLightEffect::Draw()
{
	DrawRectRotaGraph(m_pos.x, m_pos.y,
	kLightCutW * charaIdx, kLightCutH * charaIdy,//切り取り左上
	kLightCutW, kLightCutH,//切り取りの幅
	kLightScale, 180.0f * DX_PI / 180.0f, m_handle, true);

	if (charaIdy <= 1)
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y + 250,
	kDaenW * 0, kDaenH * 0,//切り取り左上
	kDaenW, kDaenH,//切り取りの幅
	kDaenScale, 0, m_handleDaen, true);
	}
	

	
}
