#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include <cmath>


namespace
{
	constexpr int ScreenWidth = 1920;
	constexpr int ScreenHeight = 1080;
	//カメラから持ってきた
	const float topPos = ScreenHeight * 0.75;

	constexpr float t = 0.1f;
}

// カメラ範囲
const float CameraScopeRangeW = 400.0f;
const float CameraScopeRangeH = 300.0f;

void InitCamera(Camera& camera)
{
	camera.pos = { ScreenWidth * 0.5f, 0.0f };
}

void StartCameraShake(Camera& camera, float power, float time)
{
	camera.shakePower = power;
	camera.shakeTimer = time;
	camera.shakeTimerMax = time;
}

void SetBossCamera(Camera& camera)
{
	//ボスシーンのカメラの中心座標
	camera.m_cameraRect.SetLT(2000,0,ScreenWidth, ScreenHeight);
	DrawBox(2000, 0, 2000 + ScreenWidth, ScreenHeight, GetColor(0, 0, 0), false);
	
}

void UpdateCamera(Camera& camera, const std::shared_ptr<Player> pPlayer)
{
	

	float camerafuture = 0;
	if (!camera.isBoss)//通常時
	{
		// 画面の中央にプレイヤーが来るようにする(x座標編)
		if (pPlayer->GetPos().x < ScreenWidth * 0.5f)//最小値のほう
		{
			camerafuture = ScreenWidth * 0.5f;
			//camera.pos.x = ScreenWidth * 0.5f;
		}
		else if (pPlayer->GetPos().x > 20000.0f - ScreenWidth * 0.5f)//最大値のほう
		{
			camerafuture = ScreenWidth * 0.5f;
			//camera.pos.x = 5000.0f - ScreenWidth * 0.5f;
		}
		else
		{
			//camera.pos.x = pPlayer->GetPos().x;
			camerafuture = pPlayer->GetPos().x;
		}
	}
	else//ボス戦
	{
		camerafuture = 2811.0f;//ボス戦の中央座標
	}

	
	camera.pos.x = std::lerp(camera.pos.x, camerafuture, t);

	
	DrawFormatString(camera.pos.x, camera.pos.y, GetColor(255, 0, 0), "camera.pos.x:%f", camera.pos.x);


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
	camera.drawOffset.y = camera.drawOffset.y - (ScreenHeight - topPos);//+(ScreenHeight * 0.5f) //ScreenHeight * 0.125f
	//カメラシェイク処理
	if (camera.shakeTimer > 0.0f)
	{
		camera.shakeTimer -= 1.0f / 60.0f;//これは1フレームで1秒ずつずらしたいからこの計算

		float progress = camera.shakeTimer / camera.shakeTimerMax;//1から0までの割合
		float currentPower = camera.shakePower * progress;

		float magX = (GetRand(200) / 100.0f - 1.0f) * currentPower;
		float magY = (GetRand(200) / 100.0f - 1.0f) * currentPower;

		camera.drawOffset.x += magX;
		camera.drawOffset.y += magY;
	}
}