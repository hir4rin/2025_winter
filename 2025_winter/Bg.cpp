#include "Bg.h"
#include "DxLib.h"
#include "Player.h"

namespace
{
	constexpr int kMapWidth = 5000;//マップ全体の幅
	constexpr int kMapHeight = 1080;//マップ全体の高さ
	constexpr int kScreenSizeWidth = 1920;
	constexpr int kScreenSizeHeight = 1080;
	const int graphHalfW = kScreenSizeWidth / 2;
	const int graphHalfH = kScreenSizeWidth / 2;
}

Bg::Bg():
	m_pos(0,0)
{
	m_bgH = LoadGraph("data/background.png");
}
Bg::~Bg()
{
}
Bg::Bg(Player* pPlayer):
	m_pPlayer(pPlayer)
{
	m_bgH = LoadGraph("data/background.png");
}

int Bg::GetScrollX()
{
	int result = static_cast<int>(m_pPlayer->GetPos().x - kScreenSizeWidth * 0.5f);
	if (result < 0)
	{
		result = 0;
	}
	if (result > kMapWidth - kScreenSizeWidth)
	{
		result = kMapWidth - kScreenSizeWidth;
	}

	return result;
}

int Bg::GetScrollY()
{
	int result = static_cast<int>(m_pPlayer->GetPos().y - kScreenSizeHeight * 0.5);
	if (result < 0)
	{
		result = 0;
	}
	if (result > kMapHeight - kScreenSizeHeight)
	{
		result = kMapHeight - kScreenSizeHeight;
	}

	return result;
}



void Bg::Init()
{

}
void Bg::Update()
{

}
void Bg::Draw()
{
	DrawBg();
}

void Bg::DrawBg()
{
//	DrawGraph(m_pos.x, m_pos.y, m_bgH, true);
	//画像サイズを取得
	//Bgのサイズ
	Size bgSize = { 0,0 };

	GetGraphSize(m_bgH, &bgSize.width, &bgSize.height);


	int scrollBg = GetScrollX() % (bgSize.width * 10);
	DrawRectRotaGraph(graphHalfW - scrollBg , graphHalfH,  // 描画位置（中心座標）
		0, 0, // 元画像の切り取り開始位置（左上）
		bgSize.width, bgSize.height,  // 切り取るサイズ（幅・高さ）
		10.0, 0, // 拡大率（1.0で等倍）// 回転角度（ラジアン）
		m_bgH, // 画像ハンドル
		false,// 透過描画フラグ（TRUEで透明色有効）
		false, false);      // 左右反転フラグ（TRUEで反転）
}
