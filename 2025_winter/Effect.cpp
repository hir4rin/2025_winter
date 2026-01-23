#include "DxLib.h"
#include "Effect.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

namespace
{
	//star
	constexpr int kStarCutW = 16;
	constexpr int kStarCutH = 16;
	constexpr float kStarScale = 1.0f;


	//dust
	constexpr int kDustCutW = 32;
	constexpr int kDustCutH = 32;
	constexpr float kDustScale = 3.0f;
	constexpr int kDustAnimDuration = 5;
	constexpr int kDustAnimNum = 6;

	//dustForClear
	constexpr int kDustForClearCutW = 80;
	constexpr int kDustForClearCutH = 64;
	constexpr float kDustForClearScale = 4.0f;
	constexpr int kDustForClearAnimDuration = 10;
	constexpr int kDustForClearAnimNum = 9;
	//blueStarLight
	constexpr int kBlueStarLightCutW = 16;
	constexpr int kBlueStarLightCutH = 16;
	constexpr int kBlueStarLightSorW = 14*16;
	constexpr int kBlueStarLightSorH = 12*16;
	constexpr float kBlueStarLightScale = 4.0f;
	constexpr int kBlueStarLightAnimDuration = 10;
	constexpr int kBlueStarLightAnimNum = 4;
	//RedStarLight
	constexpr int kRedStarLightCutW = 16;
	constexpr int kRedStarLightCutH = 16;
	constexpr int kRedStarLightSorW = 14*16;
	constexpr int kRedStarLightSorH = 12*16;
	constexpr float kRedStarLightScale = 4.0f;
	constexpr int kRedStarLightAnimDuration = 10;
	constexpr int kRedStarLightAnimNum = 4;
	//GreenStarLight
	constexpr int kGreenStarLightCutW = 16;
	constexpr int kGreenStarLightCutH = 16;
	constexpr int kGreenStarLightSorW = 14*16;
	constexpr int kGreenStarLightSorH = 12*16;
	constexpr float kGreenStarLightScale = 4.0f;
	constexpr int kGreenStarLightAnimDuration = 10;
	constexpr int kGreenStarLightAnimNum = 4;
	//Slash
	constexpr int kSlashCutW = 128;
	constexpr int kSlashCutH = 128;
	constexpr int kSlashScale = 2.0f;
	constexpr int kSlashANimDuration = 5;
	constexpr int kSlashAnimNum = 5;
	//RedLight
	constexpr int kRedLightCutW = 16;
	constexpr int kRedLightCutH = 16;
	constexpr int kRedLightSorW = 14 * 16;
	constexpr int kRedLightSorH = 3 * 16;
	constexpr float kRedLightScale = 4.0f;
	constexpr int kRedLightAnimDuration = 10;
	constexpr int kRedLightAnimNum = 4;

}

Effect::Effect(Vec2 pos,std::string name,bool dir):
	m_aliveFrame(20),
	m_starDir(true),
	charaIdx(0),
	charaIdy(0),
	m_animFrame(0)
{
	m_pos = pos;
	if (name == "star")
	{
		m_type = EffectType::Star;
		m_handle = LoadGraph("data/Game/star.png");
		assert(m_handle != -1 && "Failed to load star image");
		//m_vel.y = -10.0f;
	//ランダムなx方向の速度
		float vx = (GetRand(60) - 30) / 10.0f;//-3.0f ~ 3.0f
		float vy = (GetRand(60) - 20) / 10.0f;//-1.0f ~ 4.0f

		m_vel = { vx, vy };
	}
	if(name == "dust")
	{
		m_type = EffectType::Dust;
		m_handle = LoadGraph("data/Game/dust.png");
		//m_isRightによって向きを変える
	}
	if (name == "dustForClear")
	{
		m_pos.x += -50.0;
		m_aliveFrame = 10;
		m_type = EffectType::DustForClear;
		m_handle = LoadGraph("data/Game/dustForClear.png");
		assert(m_handle != -1 && "Failed to load  image");
	}
	if (name == "blueStarLight")
	{
		m_type = EffectType::BlueStarLight;
		m_handle = LoadGraph("data/Game/WaterEffect.png");
	}
	if (name == "redStarLight")
	{
		m_type = EffectType::RedStarLight;
		m_handle = LoadGraph("data/Game/RedEffect.png");
	}
	if (name == "greenStarLight")
	{
		m_type = EffectType::GreenStarLight;
		m_handle = LoadGraph("data/Game/GreenEffect.png");
	}
	if(name == "slash")
	{
		m_type = EffectType::Slash;
		m_handle = LoadGraph("data/Game/Effect/Slash/Slash.png");
		m_dir = dir;
	}
	if (name == "redLight")
	{
		m_type = EffectType::RedLight;
		m_handle = LoadGraph("data/Game/RedEffect.png");
	}
	
}

Effect::~Effect()
{
	DeleteGraph(m_handle);
}

void Effect::Init()
{

}
void Effect::Update()
{
	m_aliveFrame--;
	m_animFrame++;
	//effectの動き
	// 重力を受ける
	switch (m_type)
	{
		case EffectType::Star:
			// 星の動き
			m_vel.y += 0.25f;
			break;
		case EffectType::Dust:
			
			break;
		case EffectType::DustForClear:
			
			break;
		case EffectType::BlueStarLight:
			break;
		case EffectType::RedStarLight:
			break;
		case EffectType::GreenStarLight:
			break;
		case EffectType::Slash:
			break;
		case EffectType::RedLight:
			break;

	}


	

	m_pos += m_vel;
}
void Effect::Draw()
{

}

void Effect::Draw(Camera& camera)
{

	switch (m_type)
	{
	case EffectType::Star:
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
					0, 0,
				 kStarCutW, kStarCutH,//切り取りの幅
					 kStarScale, 0.0f,//左が拡大率、右が回転率
				 m_handle,
				 true,
				 m_starDir ? false : true,//反転するかどうか
				  false);
				break;
	case EffectType::Dust://Dustの描画処理
		charaIdx = (m_animFrame / kDustAnimDuration) % kDustAnimNum;


		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		 kDustCutW * charaIdx, 0,//切り取り座標
		 kDustCutW, kDustCutH,//切り取りの幅
		 kDustScale, 0.0f,//左が拡大率、右が回転率
		 m_handle,
		 true,
		 m_starDir ? false : true,//反転するかどうか
		  false);
		break;
	case EffectType::DustForClear://DustForClearの描画処理
		charaIdx = (m_animFrame / kDustForClearAnimDuration) % kDustForClearAnimNum;
		
		
			DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
				 kDustForClearCutW * charaIdx, 0,//切り取り座標
				 kDustForClearCutW, kDustForClearCutH,//切り取りの幅
				 kDustForClearScale, 0.0f,//左が拡大率、右が回転率
				 m_handle,
				 true,
				 true,//反転するかどうか
				  false);
					break;
	case EffectType::BlueStarLight://BlueStarLightの描画処理
		charaIdx = (m_animFrame / kBlueStarLightAnimDuration) % kBlueStarLightAnimNum;
		
		
			DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
			 kBlueStarLightSorW+ kBlueStarLightCutW * charaIdx, kBlueStarLightSorH,//切り取り座標
			 kBlueStarLightCutW, kBlueStarLightCutH,//切り取りの幅
			 kBlueStarLightScale, 0.0f,//左が拡大率、右が回転率
			 m_handle,
			 true,
			 true,//反転するかどうか
			  false);
			break;
	case EffectType::RedStarLight://RedStarLightの描画処理
		charaIdx = (m_animFrame / kRedStarLightAnimDuration) % kRedStarLightAnimNum;
		
		
			DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
			 kRedStarLightSorW + kRedStarLightCutW * charaIdx, kRedStarLightSorH,//切り取り座標
			 kRedStarLightCutW, kRedStarLightCutH,//切り取りの幅
			 kRedStarLightScale, 0.0f,//左が拡大率、右が回転率
			 m_handle,
			 true,
			 true,//反転するかどうか
			  false);
			break;
	case EffectType::GreenStarLight://BlueStarLightの描画処理
		charaIdx = (m_animFrame / kGreenStarLightAnimDuration) % kGreenStarLightAnimNum;
		
		
			DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
			 kGreenStarLightSorW + kGreenStarLightCutW * charaIdx, kGreenStarLightSorH,//切り取り座標
			 kGreenStarLightCutW, kGreenStarLightCutH,//切り取りの幅
			 kGreenStarLightScale, 0.0f,//左が拡大率、右が回転率
			 m_handle,
			 true,
			 true,//反転するかどうか
			  false);
			break;
	case EffectType::Slash://Slashの描画処理
		charaIdx = (m_animFrame / kSlashANimDuration) % kSlashAnimNum;

		//float offset = m_dir ? +100.0f : -100.0f;

		//DrawRectRotaGraph(m_pos.x + camera.drawOffset.x + offset, m_pos.y + camera.drawOffset.y,
		//	kSlashCutW * charaIdx, 0,//切り取り座標
		//	 kSlashCutW, kSlashCutH,//切り取りの幅
		//	 kSlashScale, 0.0f,//左が拡大率、右が回転率
		//		 m_handle,
		//		 true,
		//		 m_dir ? false : true,//反転するかどうか
		//			false);
		break;
	case EffectType::RedLight://RedLightの描画処理
		charaIdx = (m_animFrame / kRedLightAnimDuration) % kRedLightAnimNum;


		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		 kRedLightSorW + kRedLightCutW * charaIdx, kRedLightSorH,//切り取り座標
		 kRedLightCutW, kRedLightCutH,//切り取りの幅
		 kRedLightScale, 0.0f,//左が拡大率、右が回転率
		 m_handle,
		 true,
		 true,//反転するかどうか
		  false);
		break;
	}
		
	
}
