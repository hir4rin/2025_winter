#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include <cmath>


namespace
{
	const int ScreenWidth = 1920;
	const int ScreenHeight = 1080;

	constexpr float t = 0.1f;
}

// カメラ範囲
const float CameraScopeRangeW = 400.0f;
const float CameraScopeRangeH = 300.0f;

void InitCamera(Camera& camera)
{
	camera.pos = { 0.0f, 0.0f };
}

void UpdateCamera(Camera& camera, const std::shared_ptr<Player> pPlayer)
{
	//// プレイヤーの位置が、カメラの中央から一定以上離れたら
	//// カメラのポジションをその範囲内に留める
	/*if (pPlayer->GetPos().x > camera.pos.x + (CameraScopeRangeW * 0.5f))
	{
		camera.pos.x = pPlayer->GetPos().x - (CameraScopeRangeW * 0.5f);
	}
	else if (pPlayer->GetPos().x < camera.pos.x - (CameraScopeRangeW * 0.5f))
	{
		camera.pos.x = pPlayer->GetPos().x + (CameraScopeRangeW * 0.5f);
	}
	if (pPlayer->GetPos().y > camera.pos.y + (CameraScopeRangeH * 0.5f))
	{
		camera.pos.y = pPlayer->GetPos().y - (CameraScopeRangeH * 0.5f);
	}
	else if (pPlayer->GetPos().y < camera.pos.y - (CameraScopeRangeH * 0.5f))
	{
		camera.pos.y = pPlayer->GetPos().y + (CameraScopeRangeH * 0.5f);
	}
	*/

	float camerafuture = 0;

	// 画面の中央にプレイヤーが来るようにする(x座標編)
	if (pPlayer->GetPos().x < ScreenWidth * 0.5f)
	{
		camerafuture = ScreenWidth * 0.5f;
		//camera.pos.x = ScreenWidth * 0.5f;
	}
	else if (pPlayer->GetPos().x > 5000.0f - ScreenWidth * 0.5f)
	{
		camerafuture = ScreenWidth * 0.5f;
		//camera.pos.x = 5000.0f - ScreenWidth * 0.5f;
	}
	else
	{
		//camera.pos.x = pPlayer->GetPos().x;
		camerafuture = pPlayer->GetPos().x;
	}
	//camera.pos.x = std::lerp(camera.pos.x, pPlayer->GetPos().x, t);
	camera.pos.x = std::lerp(camera.pos.x, camerafuture, t);

	/*if (camera.pos.x < ScreenWidth * 0.5f)
	{
		camera.pos.x = ScreenWidth * 0.5f;
	}
	else if (camera.pos.x > 5000.0f - ScreenWidth * 0.5f)
	{
		camera.pos.x = 5000.0f - ScreenWidth * 0.5f;
	}*/


	//camera.pos.y = pPlayer->GetPos().y;

	/*printfDx("camera.pos:(%d,%d) pPlayer.pos:(%d,%d)\n",
		(int)camera.pos.x,
		(int)camera.pos.y,
		(int)pPlayer->GetPos().x,
		(int)pPlayer->GetPos().y);*/

		/////////////////////////////////////////////////////////////////////////////
		// ↓drawOffset補正
		// （カメラを動かしている感覚で他の描画物のポジションをいじれるようにする計算
		/////////////////////////////////////////////////////////////////////////////
		// ↑ベクトルや計算を使って、「カメラのポジションを動かす」
		// という感覚を保ちたいので、Draw側に足しているcamera.posをいじる
		// Draw側に足しているcamera.pos.x/yは反転させる
	camera.drawOffset.x = camera.pos.x * -1;
	camera.drawOffset.y = camera.pos.y * -1;

	// その時、画面の中央にプレイヤーが来るようにする
	// (camera.posが画面の中央になるようにする)
	camera.drawOffset.x = camera.drawOffset.x + (ScreenWidth * 0.5f);
	camera.drawOffset.y = camera.drawOffset.y;//+(ScreenHeight * 0.5f) 
}