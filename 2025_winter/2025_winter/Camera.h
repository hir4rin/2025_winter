#pragma once
#include <memory>
#include "Vec2.h"

class  Player;
struct Camera
{
	Vec2 pos;				// 実際のカメラのポジション
	Vec2 drawOffset;		// 全てのDrawObjectに足す値
};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera, const std::shared_ptr<Player> pPlayer);