#pragma once
#include <memory>
#include "Vec2.h"
#include "Rect.h"

class  Player;
struct Camera
{
	Vec2 pos;				// 実際のカメラのポジション
	Vec2 drawOffset;		// 全てのDrawObjectに足す値

	float shakePower = 0.0f;
	float shakeTimer = 0.0f;
	float shakeTimerMax = 0.0f;//減衰用のコピー

	//ボスカメラ用
	Rect m_cameraRect;
	bool isBoss = false;
	bool isFish = false;
	void ChangeIsBossTrue() { isBoss = true; }
	void ChangeIsBossFalse() { isBoss = false; }
	void ChangeIsFishTrue() { isFish = true; }
	void ChangeIsFishFalse() { isFish = false; }

};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera, const std::shared_ptr<Player> pPlayer);

/// <summary>
/// カメラを揺らす
/// </summary>
/// <param name="camera">カメラのポインタ</param>
/// <param name="power">揺れの強さ</param>
/// <param name="time">揺らす時間</param>
void StartCameraShake(Camera& camera, float power, float time);


void SetBossCamera(Camera& camera);










