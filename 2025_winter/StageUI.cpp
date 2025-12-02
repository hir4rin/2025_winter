#include "StageUI.h"
#include "DxLib.h"
#include "Camera.h"

namespace
{
	constexpr int ScreenWidth = 1920;
	constexpr int ScreenHeight = 1080;

	const float topPos = ScreenHeight * 0.75;
}


StageUI::StageUI()
{
}

StageUI::~StageUI()
{
}

void StageUI::Draw(Camera& camera)
{
	DrawBox(0, topPos, ScreenWidth, ScreenHeight, GetColor(255, 170, 80), true);
}
