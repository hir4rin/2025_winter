#pragma once
#include <memory>
#include "Vec2.h"
#include "Rect.h"

class  Player;
struct Camera
{
	Vec2 pos;				// 実際のカメラのポジション
	Vec2 drawOffset;		// 全てのDrawObjectに足す値

	//カメラ揺れ用
	float shakePower = 0.0f;
	float shakeTimer = 0.0f;
	float shakeTimerMax = 0.0f;//減衰用のコピー
	bool isShaking = false;//今カメラが揺れているかどうか

	//ボスカメラ用
	Rect m_cameraRect;
	bool isBoss = false;
	bool isFish = false;
	bool isBearWolf = false;
	void ChangeIsBossTrue() { isBoss = true; }
	void ChangeIsBossFalse() { isBoss = false; }
	//魚
	void ChangeIsFishTrue() { isFish = true; }
	void ChangeIsFishFalse() { isFish = false; }
	//熊と狼
	void ChangeIsBearWolfTrue() { isBearWolf = true; }
	void ChangeIsBearWolfFalse() { isBearWolf = false; }

	//カメラの右側の最大値
	float cameraRightMax = 20000.0f - 1920.0f / 2.0f;

};

void InitCamera(Camera& camera,int stageNum);
void UpdateCamera(Camera& camera, const std::shared_ptr<Player> pPlayer);

/// <summary>
/// カメラを揺らす
/// </summary>
/// <param name="camera">カメラのポインタ</param>
/// <param name="power">揺れの強さ</param>
/// <param name="time">揺らす時間</param>
void StartCameraShake(Camera& camera, float power, float time);


void SetBossCamera(Camera& camera);










