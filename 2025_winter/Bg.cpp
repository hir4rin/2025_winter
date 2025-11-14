#include "Bg.h"
#include "DxLib.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include "Rect.h"



namespace
{
	constexpr int kMapWidth = 5000;//マップ全体の幅
	constexpr int kMapHeight = 1080;//マップ全体の高さ
	constexpr int kScreenSizeWidth = 1920;
	constexpr int kScreenSizeHeight = 1080;
	const int graphHalfW = kScreenSizeWidth / 2;
	const int graphHalfH = kScreenSizeWidth / 2;

	constexpr int kChipNumX = 60;//チップの数X
	constexpr int kChipNumY = 17;//チップの数Y

	constexpr int kChipSize = 16;		// マップチップ1つのサイズ
	constexpr float kChipScale = 4.0f; 	// マップチップ拡大率
}

Bg::Bg():
	m_pos(0,0)
{
	m_bgH = LoadGraph("data/background.png");
}
Bg::~Bg()
{
	DeleteGraph(m_bgH);
	DeleteGraph(m_mapH);
}
Bg::Bg(Player* pPlayer):
	m_pPlayer(pPlayer),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_chipData()
	
{
	m_bgH = LoadGraph("data/background.png");
	m_mapH = LoadGraph("data/map.png");

	// 画像のマップチップ数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_mapH, &graphW, &graphH);

	m_graphChipNumX = graphW / kChipSize;
	m_graphChipNumY = graphH / kChipSize;

	// マップデータを読み込む
	LoadMapData();
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
	DrawMapChip();
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

void Bg::LoadMapData()
{
	std::ifstream file("data/map1.csv");
	std::string line;

	// getline関数で1行ずつ読み込む
	int y = 0;
	while (std::getline(file, line) && y < kChipNumY)
	{
		std::istringstream stream(line);
		std::string field;

		// 「,」区切りごとにデータを読み込む
		int x = 0;
		while (getline(stream, field, ',') && x < kChipNumX)
		{
			// 文字列をint型に変換してm_chipDataに追加する
			m_chipData[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::DrawMapChip()
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize * kChipScale - GetScrollX());
			int posY = static_cast<int>(y * kChipSize * kChipScale - GetScrollY());

			// 画面外は描画しない
			if (posX < 0 - kChipSize) continue;
			if (posX > kScreenSizeWidth) continue;
			if (posY < 0 - kChipSize) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData[x][y];
			//if (chipNo == 5) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize * (chipNo % m_graphChipNumX);
			int srcY = kChipSize * (chipNo / m_graphChipNumX);//???

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize * kChipScale * 0.5f),
				static_cast<int>(posY + kChipSize * kChipScale * 0.5f),
				srcX, srcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			// 当たり判定
			DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale, 0x00ff00, false);
#endif
		}
	}
}

bool Bg::IsCollision(Rect rect, Rect& chipRect)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData[x][y] == 5) continue;

			int chipLeft = static_cast<int>(x * kChipSize * kChipScale);
			int chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			int chipTop = static_cast<int>(y * kChipSize * kChipScale);
			int chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);

			// 絶対に当たらない場合
			if (chipLeft > rect.GetRight()) continue;
			if (chipTop > rect.GetBottom()) continue;
			if (chipRight < rect.Getleft()) continue;
			if (chipBottom < rect.GetTop()) continue;

			// ぶつかったマップチップの矩形を設定する
			chipRect.m_left = static_cast<float>(chipLeft);
			chipRect.m_right = static_cast<float>(chipRight);
			chipRect.m_top = static_cast<float>(chipTop);
			chipRect.m_bottom = static_cast<float>(chipBottom);

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;

}

