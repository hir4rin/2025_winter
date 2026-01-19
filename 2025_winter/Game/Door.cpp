#include "Door.h"
#include "DxLib.h"
#include "Camera.h"
#include <cassert>

namespace
{
	constexpr int kCharaWidth = 16;
	constexpr int kCharaHeight = 32;
	constexpr float kDoorScale = 5.0f;

	constexpr int kDoor2Width = 64*4;
	constexpr int kDoor2Height = 64*4;
	constexpr float kDoor2Scale = 1.0f;
	constexpr int kDoor2Num = 10;


	constexpr int kStarWidth = 32;
	constexpr int kStarHeight = 32;
	constexpr float kStarScale = 2.0f;
	constexpr int kStarNum = 13;
}

Door::Door(Vec2 pos):
	m_frame(0),
	m_pos(pos)
{
	m_handle = LoadGraph("data/doors.png");
	assert(m_handle >= 0);

	charaIdx = 0;
	charaIdy = 0;
}

Door::Door(Vec2 pos,StageID id):
	m_frame(0),
	m_pos(pos)
{
	
	m_handle = LoadGraph("data/doors.png");
	assert(m_handle >= 0);

	charaIdx = 0;
	charaIdy = 0;
	m_id = id;
}

Door::Door(Vec2 pos, StageID id, int num):
	m_frame(0),
	m_bossFrame(0),
	m_pos(pos),
	m_starHandle(-1)
{
	m_handle = LoadGraph("data/doors.png");
	m_starHandle = LoadGraph("data/Game/StarPoint.png");
	assert(m_handle >= 0);
	assert(m_starHandle >= 0);

	charaIdx = 0;
	charaIdy = 0;
	starIdx = 0;
	m_id = id;
	m_isStageSelect = true;
	m_stage = num;
}

Door::~Door()
{
	DeleteGraph(m_handle);
	DeleteGraph(m_starHandle);
}

void Door::Init()
{

}
void Door::Update()
{
	m_bossFrame++;

	m_colRect.SetCenter(m_pos.x,m_pos.y,kCharaWidth* kDoorScale,kCharaHeight* kDoorScale);
}
void Door::Draw(Camera& camera)
{

	if (m_isStageSelect)//ステージセレクトシーンのドア
	{
		starIdx = m_bossFrame / 15 % (kStarNum);

		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y,
		kCharaWidth * charaIdx, kCharaHeight * charaIdy,//切り取り左上
		kCharaWidth, kCharaHeight,//切り取りの幅
		kDoorScale, 0.0f, m_handle, true, false);

		
	
		StarDraw(camera);

	}
	else//ほかのドア
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y,
		kCharaWidth * charaIdx, kCharaHeight * charaIdy,//切り取り左上
		kCharaWidth, kCharaHeight,//切り取りの幅
		kDoorScale, 0.0f, m_handle, true, false);
	}

	

#ifdef _DEBUG
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);
#endif // _DEBUG
}

void Door::OutUpdate()
{
	//fade_interval = 60
	m_frame++;

	if (m_isStageSelect)//ステージセレクトシーンのドア
	{
		charaIdx = m_frame / 15 % 3;
		if (m_frame >= 45) charaIdx = 2;
 
	}
	else
	{
		charaIdx = m_frame / 15 % 3;
		if (m_frame >= 45) charaIdx = 2;
	}


	

}

void Door::StarDraw(Camera& camera)
{
	switch (m_stage)
	{
	case 1:
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
			kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
			kStarWidth, kStarHeight,//切り取りの幅
			kStarScale, 0.0f, m_starHandle, true, false);
		break;
	case 2:
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x - kCharaWidth * 1.0f,
		m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
		kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
		kStarWidth, kStarHeight,//切り取りの幅
		kStarScale, 0.0f, m_starHandle, true, false);
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + kCharaWidth * 1.0f,
		m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
		kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
		kStarWidth, kStarHeight,//切り取りの幅
		kStarScale, 0.0f, m_starHandle, true, false);
		break;
	case 3:
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x - kCharaWidth * 2.0f,
		m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 10,
		kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
		kStarWidth, kStarHeight,//切り取りの幅
		kStarScale, 0.0f, m_starHandle, true, false);
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
		m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
		kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
		kStarWidth, kStarHeight,//切り取りの幅
		kStarScale, 0.0f, m_starHandle, true, false);
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + kCharaWidth * 2.0f,
		m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 10,
		kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
		kStarWidth, kStarHeight,//切り取りの幅
		kStarScale, 0.0f, m_starHandle, true, false);
		break;
	case 4:
		// 青くする
		SetDrawBright(255, 0, 255);
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
			kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
			kStarWidth, kStarHeight,//切り取りの幅
			kStarScale, 0.0f, m_starHandle, true, false);
		// 描画色を元に戻す
		SetDrawBright(255, 255, 255);
		break;
	case 5:
			// 青くする
		SetDrawBright(150, 255, 150); // 回復・草
		//星
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x,
			m_pos.y + camera.drawOffset.y - kCharaHeight * kDoorScale * 1.0f / 2.0f - 20,
			kStarWidth * starIdx, kStarHeight * 0,//切り取り左上
			kStarWidth, kStarHeight,//切り取りの幅
			kStarScale, 0.0f, m_starHandle, true, false);
		// 描画色を元に戻す
		SetDrawBright(255, 255, 255);
		break;
	}

}
