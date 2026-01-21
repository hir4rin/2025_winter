#include "Cannon.h"
#include "DxLib.h"
#include "Camera.h"
#include <cassert>

namespace
{
	//TransFormアニメーション関連
	constexpr int kCannonTransFormCutW = 50;//切り出し座標
		constexpr int kCannonTransFormCutH = 30;//切り出し座標
		constexpr int kCannonTransFormSorcH = 50;//切り取りY座標
		constexpr int  kCannonTransFormNum = 6;
		constexpr int  kCannonDuration = 10;
		//Fireアニメーション関連
		constexpr int kCannonFireCutW = 60;//切り出し座標
		constexpr int kCannonFireCutH = 80;//切り出し座標
		constexpr int kCannonFireSorcH = 32;//切り取りY座標
		constexpr int  kCannonFireNum = 8;
		constexpr int  kCannonFireDuration = 5;
		constexpr float drawOffSetY = 50.0f;//描画オフセットY座標

		//大砲
		constexpr int  kCannonWidth = 500;
		constexpr int kCannonHeight = 550;

	constexpr float  kFrozenScale = 5.0f;
	
}




Cannon::Cannon():
	charaIdx(0),
	charaIdy(0),
	m_animFrame(0),
	m_pos(400.0f, 720.0f)
{
	 m_handle = LoadGraph("data/Cannon.png");
	assert(m_handle != -1 && "Cannon画像の読み込みに失敗しました");
	m_colRect.SetCenter(0.0f, 0.0f, 64.0f, 64.0f);
	m_state = CannonState::Idle;
}

Cannon::~Cannon()
{
}

void Cannon::Init()
{
}
void Cannon::Update()
{
	m_animFrame++;
	//switch (m_state)
	//{
	//	case CannonState::Idle://待機
	//	//待機状態の処理
	//		break;
	//	case CannonState::Transform://変形
	//		//最後のコマで止める
	//		if (charaIdx == kCannonTransFormNum - 1)
	//		{
	//			charaIdx = kCannonTransFormNum - 1;
	//		}
	//		else//アニメーション進行 
	//			charaIdx = (m_animFrame / kCannonDuration) % kCannonTransFormNum;
	//		
	//		break;
	//	case CannonState::Fire://発射
	//		//最後のコマで止める
	//		if (charaIdx == kCannonFireNum - 1)
	//		{
	//			charaIdx = kCannonFireNum - 1;
	//		}
	//		else//アニメーション進行
	//			charaIdx = (m_animFrame / kCannonFireDuration) % kCannonFireNum;
	//			
	//			break;
	//}
	m_colRect.SetCenter(m_pos.x, m_pos.y, 64.0f*2, 64.0f * 2);

}
void Cannon::Draw(Camera& camera)
{
	
	

#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(255, 255, 255), false);
#endif
	//switch (m_state)
	//{
	//	case CannonState::Idle:
	//		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	//	kCannonTransFormCutW * 0, kCannonTransFormCutH,//切り取り左上
	//	kCannonTransFormCutW, kCannonTransFormSorcH,//切り取りの幅
	//	kFrozenScale, 0.0f, m_handle, true, true);
	//		break;
	//	case CannonState::Transform:
	//		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
	//	kCannonTransFormCutW * charaIdx, kCannonTransFormCutH,//切り取り左上
	//	kCannonTransFormCutW, kCannonTransFormSorcH,//切り取りの幅
	//	kFrozenScale, 0.0f, m_handle, true, true);
	//		break;
	//	case CannonState::Fire:
	//		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y+ drawOffSetY,
	//	kCannonFireCutW * charaIdx, kCannonFireCutH,//切り取り左上
	//	kCannonFireCutW, kCannonFireSorcH,//切り取りの幅
	//	kFrozenScale, 0.0f, m_handle, true, false);
	//		break;
	//}
	
	DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y + 43.0f,
	kCannonTransFormCutW * 0, 0,//切り取り左上
	kCannonWidth, kCannonHeight,//切り取りの幅
	0.3f, 0.0f, m_handle, true, false);
}
void Cannon::Fire()
{
}
