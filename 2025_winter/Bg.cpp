#include "Bg.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include "Rect.h"



namespace
{
	constexpr int kMapWidth = 5000;//マップ全体の幅
	constexpr int kMapHeight = 1080;//マップ全体の高さ
	constexpr int kScreenSizeWidth = 1920;
	constexpr int kScreenSizeHeight = 1080;
	const int graphHalfW = kScreenSizeWidth / 2;
	const int graphHalfH = kScreenSizeHeight / 2;

	constexpr int kChipNumX = 300;//チップの数X
	constexpr int kChipNumY = 17;//チップの数Y

	//1_1
	constexpr int kChipSize = 16;		// マップチップ1つのサイズ
	constexpr float kChipScale = 4.0f; 	// マップチップ拡大率
	//1_2
	constexpr int kChipSize2 = 32;		// マップチップ1つのサイズ
	constexpr float kChipScale2 = 2.0f; 	// マップチップ拡大率


	//床貫通のタイルのサイズ
	constexpr float kThrough = 0.25f;


	//背景
	constexpr float kBgScale = 3.0f;

	//カメラから持ってきた
	const float topPos = kScreenSizeHeight * 0.75;
	//カメラのUI上げ分
	const float cameraYoffset = kScreenSizeHeight - topPos;
	


}

Bg::Bg() :
	m_pos(0, 0)

{
	//m_bgH = LoadGraph("data/background.png");
}
Bg::~Bg()
{
	DeleteGraph(m_fadeH);
	DeleteGraph(m_bgH);
	DeleteGraph(m_bgH2);
	DeleteGraph(m_bgH3);
	DeleteGraph(m_bgH4);
	DeleteGraph(m_bgH5);
	DeleteGraph(m_bgH6);
	DeleteGraph(m_bgH7);
	DeleteGraph(m_bgH8);
	DeleteGraph(m_mapH);
}
Bg::Bg(std::shared_ptr<Player> pPlayer, int stagenum) :
	m_pPlayer(pPlayer),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_chipData(),
	StageNum(stagenum),
	m_extRate(0.5)
{
	//フェード
	m_fadeH = LoadGraph("data/fade.png");
	assert(m_fadeH >= 0);
	//一旦試す
	//背景も後々変える
	switch (StageNum)
	{
	case 0:
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_1/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_1/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_1/3.png");
		m_bgH4 = LoadGraph("data/Back/nature_1/4.png");
		m_bgH5 = LoadGraph("data/Back/nature_1/5.png");
		m_bgH6 = LoadGraph("data/Back/nature_1/6.png");
		m_bgH7 = LoadGraph("data/Back/nature_1/7.png");
		m_bgH8 = LoadGraph("data/Back/nature_1/8.png");
		break;
	case 1://1_1
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_1/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_1/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_1/3.png");
		m_bgH4 = LoadGraph("data/Back/nature_1/4.png");
		m_bgH5 = LoadGraph("data/Back/nature_1/5.png");
		m_bgH6 = LoadGraph("data/Back/nature_1/6.png");
		m_bgH7 = LoadGraph("data/Back/nature_1/7.png");
		m_bgH8 = LoadGraph("data/Back/nature_1/8.png");
		break;
	case 2://1_2
		m_mapH = LoadGraph("data/Stage/map1_2.png");
		m_bgH = LoadGraph("data/Back/nature_7/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_7/2.png");
		break;
	case 3://1_3
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_3/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_3/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_3/3.png");
		m_bgH4 = LoadGraph("data/Back/nature_3/4.png");
		break;
	case 4://クリアシーン
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/1.png");
		m_bgH2 = LoadGraph("data/Back/2.png");
		m_bgH3= LoadGraph("data/Back/3.png");
		m_bgH4= LoadGraph("data/Back/4.png");
		break;

	case 6://2_1
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_2/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_2/2.png");
		//m_bgH3 = LoadGraph("data/Back/nature_2/3.png");
		//m_bgH4 = LoadGraph("data/Back/nature_2/4.png");
		break;
	case 7://2_2
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_2/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_2/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_2/3.png");
		//m_bgH4 = LoadGraph("data/Back/nature_2/4.png");
		break;
	case 8://2_3
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH = LoadGraph("data/Back/nature_2/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_2/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_2/3.png");
		m_bgH4 = LoadGraph("data/Back/nature_2/4.png");
		break;
	case 11:
		m_mapH = LoadGraph("data/Stage/map.png");
		m_bgH =	 LoadGraph("data/Back/nature_2/1.png");
		m_bgH2 = LoadGraph("data/Back/nature_2/2.png");
		m_bgH3 = LoadGraph("data/Back/nature_2/3.png");
		m_bgH4 = LoadGraph("data/Back/nature_2/4.png");
		break;
	default:
		assert(false && "Bgの画像読み込みに失敗");
		break;
	}
	assert(m_mapH >= 0);
	assert(m_bgH >= 0);

	


	// 画像のマップチップ数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_mapH, &graphW, &graphH);
	switch (StageNum)
	{
	case 0:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 1:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 2:
		m_graphChipNumX = graphW / kChipSize2;
		m_graphChipNumY = graphH / kChipSize2;
		break;
	case 3:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 4:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 6://2_1
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 7:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 8:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
	case 11:
		m_graphChipNumX = graphW / kChipSize;
		m_graphChipNumY = graphH / kChipSize;
		break;
		
	default:
		assert(false && "マップチップの画像読み込みに失敗");

		break;

	}


	// マップデータを読み込む

	switch (StageNum)
	{
	case 0://stageSelect
		LoadMapData();
		break;
	case 1://最初のマップ
		LoadMapData0();
		break;
	case 2:
		LoadMapData1_2();
		break;
	case 3:
		LoadMapData1_3();
		break;
	case 4:
		LoadMapDataClear();
		break;
	case 6:    //2_1
		LoadMapData2_1();
		break;
	case 7:
		LoadMapData2_2();
		break;
	case 8:
		LoadMapData2_3();
		break;
	case 11:
		LoadMapDataSub();
		break;
	default:
		assert(false && "Bgのマップデータ読み込みに失敗");
		break;
	}

}

void Bg::FadeInBg(Camera& camera)
{
	m_extRate += 0.2;

	//画像サイズを取得
	//Bgのサイズ
	Size bgSize = { 0,0 };

	GetGraphSize(m_fadeH, &bgSize.width, &bgSize.height);

	//画像の4てんの座標を求める用

	float cx = m_pPlayer->GetPos().x + camera.drawOffset.x;
	float cy = m_pPlayer->GetPos().y + camera.drawOffset.y;

	float halfW = bgSize.width * m_extRate / 2.0f;
	float halfH = bgSize.height * m_extRate / 2.0f;

	float leftX = cx - halfW;
	float rightX = cx + halfW;
	float topY = cy - halfH;
	float bottomY = cy + halfH;

	//左上
	DrawBox(0, 0, rightX, topY, GetColor(0, 0, 0), true);
	//左下
	DrawBox(0, kScreenSizeHeight, leftX, topY, GetColor(0, 0, 0), true);
	//右上
	DrawBox(rightX,0,kScreenSizeWidth,bottomY, GetColor(0, 0, 0), true);
	//右下
	DrawBox(leftX,bottomY,kScreenSizeWidth,kScreenSizeHeight, GetColor(0, 0, 0), true);

	DrawRectRotaGraph(m_pPlayer->GetPos().x + camera.drawOffset.x, m_pPlayer->GetPos().y + camera.drawOffset.y,
		0, 0,
		bgSize.width, bgSize.height,
		m_extRate, 0,//拡大率、回転
		m_fadeH, true);

}

void Bg::FadeOutBg(Camera& camera)
{
	m_extRate -= 0.2;

	//画像サイズを取得
	//Bgのサイズ
	Size bgSize = { 0,0 };

	GetGraphSize(m_fadeH, &bgSize.width, &bgSize.height);

	//画像の4てんの座標を求める用

	float cx = m_pPlayer->GetPos().x + camera.drawOffset.x;
	float cy = m_pPlayer->GetPos().y + camera.drawOffset.y;

	float halfW = bgSize.width * m_extRate / 2.0f;
	float halfH = bgSize.height * m_extRate / 2.0f;

	float leftX = cx - halfW;
	float rightX = cx + halfW;
	float topY = cy - halfH;
	float bottomY = cy + halfH;

	//左上
	DrawBox(0, 0, rightX, topY, GetColor(0, 0, 0), true);
	//左下
	DrawBox(0, kScreenSizeHeight, leftX, topY, GetColor(0, 0, 0), true);
	//右上
	DrawBox(rightX, 0, kScreenSizeWidth, bottomY, GetColor(0, 0, 0), true);
	//右下
	DrawBox(leftX, bottomY, kScreenSizeWidth, kScreenSizeHeight, GetColor(0, 0, 0), true);

	DrawRectRotaGraph(m_pPlayer->GetPos().x + camera.drawOffset.x, m_pPlayer->GetPos().y + camera.drawOffset.y,
		0, 0,
		bgSize.width, bgSize.height,
		m_extRate, 0,//拡大率、回転
		m_fadeH, true);
}




void Bg::Init()
{

}
void Bg::Update()
{

}
void Bg::Draw(Camera& camera)
{
	DrawBg(camera);

	switch (StageNum)
	{
	case 0://stage0
		DrawMapChip0(camera);
		break;
	case 1://最初のマップ
		DrawMapChip0(camera);
		break;
	case 2://1_2のマップ
		DrawMapChip1_2(camera);
		break;
	case 3://1_3のマップ
		DrawMapChip1_3(camera);
		break;
	case 4://stageClearのマップ
		DrawMapChipClear(camera);
		break;
	case 6://2_1のマップ
		DrawMapChip1_3(camera);
		break;
	case 7://2_2のマップ
		DrawMapChip1_3(camera);
		break;
	case 8://2_3のマップ
		DrawMapChip1_3(camera);
		break;
	case 11://fishのマップ
		DrawMapChip1_3(camera);
		break;
	default:
		assert(false && "Bgのマップチップ描画に失敗");
		break;
	}
}

void Bg::BgHSetDraw(Camera& camera, int handle, float rate)
{


	//画像サイズを取得
	//Bgのサイズ
	Size bgSize = { 0,0 };

	GetGraphSize(handle, &bgSize.width, &bgSize.height);

	//どのくらい普通より上で描画するのか
	float rateY = 0.5f;
	switch (StageNum)
	{
	case 0:
		rateY = 0.8f;
		break;
	case 1://1_1
		rateY = 0.8f;
		break;
	case 2://1_2
		rateY = 1.0f;
		break;
	case 3://1_3
		rateY = 1.0f;
		break;
	case 4://クリアシーン
		rateY = 1.0f;
		break;
	case 6://2_1
		rateY = 1.0f;
		break;
	case 7://2_2
		rateY = 1.0f;
		break;
	case 8://2_3
		rateY = 1.0f;
		break;
	case 11://
		rateY = 1.0f;
		break;
	default:
		assert(false && "BgのBgHSetDrawで失敗");
		break;
	}


	float scaleX = static_cast<float>(kScreenSizeWidth) / bgSize.width;
	float scaleY = static_cast<float>(kScreenSizeHeight * rateY) / bgSize.height;

	float scale = max(scaleX, scaleY);

	//ループ処理
	float drawWidth = bgSize.width * scale;

	//剰余でループ
	int looped = static_cast<int>(camera.drawOffset.x * rate) % static_cast<int>(drawWidth);

	//float に戻す
	float bgX = static_cast<float>(looped);

	////どれぐらい暗くするのか
	//SetDrawBright(c, c, c); // ← 遠景なら暗め(255がMax正常値)

	DrawRectRotaGraph(graphHalfW + bgX, graphHalfH + (camera.drawOffset.y+0 ) - kScreenSizeHeight * (1- rateY),  // 描画位置（中心座標）
		0, 0, // 元画像の切り取り開始位置（左上）
		bgSize.width, bgSize.height,  // 切り取るサイズ（幅・高さ）
		scale, 0, // 拡大率（1.0で等倍）// 回転角度（ラジアン）
		handle, // 画像ハンドル
		true,// 透過描画フラグ（TRUEで透明色有効）
		false, false);      // 左右反転フラグ（TRUEで反転）

	// 2枚目（右側）
	DrawRectRotaGraph(
		graphHalfW + bgX + drawWidth, graphHalfH + (camera.drawOffset.y + 0) - kScreenSizeHeight * (1 - rateY),
		0, 0,
		bgSize.width, bgSize.height,
		scale, 0,
		handle,
		TRUE,
		FALSE, FALSE
	);
	
	//SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
}

void Bg::DrawBg(Camera& camera)
{
	int t = 170;
	int m = 140;
	int b = 190;
	
	switch (StageNum)
	{
	case 0:
		SetDrawBright(t, t, t);// ← 中景
		BgHSetDraw(camera, m_bgH, 0.0);
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		if (m_bgH5 != -1)BgHSetDraw(camera, m_bgH5, 0.3);
		if (m_bgH6 != -1)BgHSetDraw(camera, m_bgH6, 0.5);
		if (m_bgH7 != -1)BgHSetDraw(camera, m_bgH7, 0.5);
		//SetDrawBright(255, 255, 255);// ← 前景
		if (m_bgH8 != -1)BgHSetDraw(camera, m_bgH8, 1.0);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す

		break;
	case 1://1_1
	
		SetDrawBright(t, t, t);// ← 中景
		BgHSetDraw(camera, m_bgH, 0.0);
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		if (m_bgH5 != -1)BgHSetDraw(camera, m_bgH5, 0.3);
		if (m_bgH6 != -1)BgHSetDraw(camera, m_bgH6, 0.5);
		if (m_bgH7 != -1)BgHSetDraw(camera, m_bgH7, 0.5);
		//SetDrawBright(255, 255, 255);// ← 前景
		if (m_bgH8 != -1)BgHSetDraw(camera, m_bgH8, 1.0);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す

		break;
	case 2://1_2
		SetDrawBright(t, t, t);// ← 中景
		BgHSetDraw(camera, m_bgH, 0.0);
	SetDrawBright(b, b, b);// ← 中景
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.6);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
	case 3://1_3
		SetDrawBright(t, t, t); // ← 遠景なら暗め
		BgHSetDraw(camera, m_bgH, 0.0);
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.1);
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
	case 4://クリアシーン
		BgHSetDraw(camera, m_bgH, 0.0);
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		if (m_bgH5 != -1)BgHSetDraw(camera, m_bgH5, 0.3);
		if (m_bgH6 != -1)BgHSetDraw(camera, m_bgH6, 0.5);
		if (m_bgH7 != -1)BgHSetDraw(camera, m_bgH7, 0.5);
		if (m_bgH8 != -1)BgHSetDraw(camera, m_bgH8, 1.0);
		break;

	case 6://2_1
		BgHSetDraw(camera, m_bgH, 0.0);
		SetDrawBright(t, t, t);// ← 中景
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
	case 7://2_2
		BgHSetDraw(camera, m_bgH, 0.0);
		SetDrawBright(t, t, t);// ← 中景
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
		break;
	case 8://2_3
		BgHSetDraw(camera, m_bgH, 0.0);
		SetDrawBright(t, t, t);// ← 中景
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
	case 11://2_3
		BgHSetDraw(camera, m_bgH, 0.0);
		SetDrawBright(t, t, t);// ← 中景
		if (m_bgH2 != -1)BgHSetDraw(camera, m_bgH2, 0.3);
		SetDrawBright(b, b, b);// ← 中景
		if (m_bgH3 != -1)BgHSetDraw(camera, m_bgH3, 0.3);
		if (m_bgH4 != -1)BgHSetDraw(camera, m_bgH4, 0.3);
		SetDrawBright(255, 255, 255); // ← ★必ず元に戻す
		break;
	
	default:
		assert(false && "Bgの背景スクロールに失敗");
		break;
	}


}
//ステージセレクと
void Bg::LoadMapData()
{
	std::ifstream file("data/Stage/StageSelect.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
//1_1のマップデータ
void Bg::LoadMapData0()
{
	std::ifstream file("data/Stage/stage1_1.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapData1_2()
{
	std::ifstream file("data/Stage/stage1_2.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapData1_3()
{
	std::ifstream file("data/Stage/stage1_3.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapDataClear()
{
	std::ifstream file("data/Stage/stageClear.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapData2_1()
{
	std::ifstream file("data/Stage/stage2_1.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapData2_2()
{
	std::ifstream file("data/Stage/stage2_2.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapData2_3()
{
	std::ifstream file("data/Stage/stage2_3.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::LoadMapDataSub()
{
	std::ifstream file("data/Stage/FishStage.csv");
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
			m_chipData0[x][y] = std::stoi(field);
			x++;
		}
		y++;
	}
}
void Bg::DrawMapChip0(Camera& camera)
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize * kChipScale + camera.drawOffset.x);
			int posY = static_cast<int>(y * kChipSize * kChipScale + camera.drawOffset.y);

			// 画面外は描画しない
			if (posX < 0 - kChipSize * kChipScale) continue;
			if (posX > kScreenSizeWidth + kChipSize * kChipScale) continue;
			if (posY < 0 - kChipSize) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData0[x][y];
			if (chipNo == 0) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize * (chipNo % m_graphChipNumX);
			int srcY = kChipSize * (chipNo / m_graphChipNumX);

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize * kChipScale * 0.5f),
				static_cast<int>(posY + kChipSize * kChipScale * 0.5f),
				srcX, srcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			bool clearnness = (m_chipData0[x][y] == 39 ||
						   m_chipData0[x][y] == 40 ||
						   m_chipData0[x][y] == 41);
			if (clearnness)
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale* kThrough, 0x00ff00, false);
			}
			else
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale, 0x00ff00, false);
			}
		
#endif
		}
	}
}
void Bg::DrawMapChip1_2(Camera& camera)
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize2 * kChipScale2 + camera.drawOffset.x);
			int posY = static_cast<int>(y * kChipSize2 * kChipScale2 + camera.drawOffset.y);

			// 画面外は描画しない
			if (posX < 0 - kChipSize2 * kChipScale) continue;
			if (posX > kScreenSizeWidth + kChipSize * kChipScale) continue;
			if (posY < 0 - kChipSize2) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData0[x][y];
			if (chipNo == 3) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize2 * (chipNo % m_graphChipNumX);
			int srcY = kChipSize2 * (chipNo / m_graphChipNumX);

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize2 * kChipScale2 * 0.5f),
				static_cast<int>(posY + kChipSize2 * kChipScale2 * 0.5f),
				srcX, srcY,
				kChipSize2, kChipSize2,
				kChipScale2, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			// 当たり判定
			DrawBoxAA(posX, posY, posX + kChipSize2 * kChipScale2, posY + kChipSize2 * kChipScale2, 0x00ff00, false);
#endif
		}
	}
}
void Bg::DrawMapChip1_3(Camera& camera)
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize * kChipScale + camera.drawOffset.x);
			int posY = static_cast<int>(y * kChipSize * kChipScale + camera.drawOffset.y);

			// 画面外は描画しない
			if (posX < 0 - kChipSize * kChipScale) continue;
			if (posX > kScreenSizeWidth + kChipSize * kChipScale) continue;
			if (posY < 0 - kChipSize) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData0[x][y];
			if (chipNo == 0) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize * (chipNo % m_graphChipNumX);
			int srcY = kChipSize * (chipNo / m_graphChipNumX);

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize * kChipScale * 0.5f),
				static_cast<int>(posY + kChipSize * kChipScale * 0.5f),
				srcX, srcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			if (clearnness)
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale * kThrough, 0x00ff00, false);
			}
			else
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale, 0x00ff00, false);
			}

#endif
		}
	}
}
void Bg::DrawMapChipClear(Camera& camera)
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize * kChipScale + camera.drawOffset.x);
			int posY = static_cast<int>(y * kChipSize * kChipScale + camera.drawOffset.y);

			// 画面外は描画しない
			if (posX < 0 - kChipSize * kChipScale) continue;
			if (posX > kScreenSizeWidth + kChipSize * kChipScale) continue;
			if (posY < 0 - kChipSize) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData0[x][y];
			if (chipNo == 0) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize * (chipNo % m_graphChipNumX);
			int srcY = kChipSize * (chipNo / m_graphChipNumX);

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize * kChipScale * 0.5f),
				static_cast<int>(posY + kChipSize * kChipScale * 0.5f),
				srcX, srcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			bool clearnness = (m_chipData0[x][y] == 39 ||
						   m_chipData0[x][y] == 40 ||
						   m_chipData0[x][y] == 41);
			if (clearnness)
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale * kThrough, 0x00ff00, false);
			}
			else
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale, 0x00ff00, false);
			}

#endif
		}
	}
}
void Bg::DrawMapChip(Camera& camera)
{
	// マップチップの描画
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x * kChipSize * kChipScale + camera.drawOffset.x);
			int posY = static_cast<int>(y * kChipSize * kChipScale + camera.drawOffset.y);

			// 画面外は描画しない
			if (posX < 0 - kChipSize * kChipScale) continue;
			if (posX > kScreenSizeWidth + kChipSize * kChipScale) continue;
			if (posY < 0 - kChipSize) continue;
			if (posY > kScreenSizeHeight) continue;

			// 設置するチップ
			int chipNo = m_chipData0[x][y];
			if (chipNo == 0) continue; // チップ番号5は空白なので描画しない

			// マップチップのグラフィック切り出し座標
			int srcX = kChipSize * (chipNo % m_graphChipNumX);
			int srcY = kChipSize * (chipNo / m_graphChipNumX);

			DrawRectRotaGraph(
				static_cast<int>(posX + kChipSize * kChipScale * 0.5f),
				static_cast<int>(posY + kChipSize * kChipScale * 0.5f),
				srcX, srcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapH, true);

#ifdef _DEBUG
			bool clearnness = (m_chipData0[x][y] == 39 ||
						   m_chipData0[x][y] == 40 ||
						   m_chipData0[x][y] == 41);
			if (clearnness)
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale * kThrough, 0x00ff00, false);
			}
			else
			{
				// 当たり判定
				DrawBoxAA(posX, posY, posX + kChipSize * kChipScale, posY + kChipSize * kChipScale, 0x00ff00, false);
			}

#endif
		}
	}
}

bool Bg::IsCollision(Rect rect, Rect& chipRect)
{
	bool ans;
	//マップごとに判定を変える
	switch (StageNum)
	{
	case 0:
		ans = IsCollision1_1(rect, chipRect);
		break;
	case 1://1_1のマップ
		ans = IsCollision1_1(rect, chipRect);
		break;
	case 2://1_2のマップ
		ans = IsCollision1_2(rect, chipRect);
		break;
	case 3://1_3のマップ
		ans = IsCollision1_3(rect, chipRect);
		break;
	case 4://1_4のマップ
		ans = IsCollisionClear(rect, chipRect);
		break;
	case 6:
		ans = IsCollision1_3(rect, chipRect);
		break;
	case 7:
		ans = IsCollision1_3(rect, chipRect);
		break;
	case 8:
		ans = IsCollision1_3(rect, chipRect);
		break;
	case 11:
		ans = IsCollision1_3(rect, chipRect);
		break;
	default:
		assert(false && "ここはIsCollisionの判定を変えるところです");

		break;
	}



	return ans;

}
bool Bg::IsCollision1_1(Rect rect, Rect& chipRect)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 5) continue;
			if (m_chipData0[x][y] == 0) continue;


			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			int chipLeft;
			int chipRight;
			int chipTop ;
			int chipBottom;

			if (clearnness)
			{
				 chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				 chipTop = static_cast<int>(y * kChipSize * kChipScale);
				 chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale * kThrough);
				 chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			}
			else
			{

				 chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				 chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
				 chipTop = static_cast<int>(y * kChipSize * kChipScale);
				 chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);
			}
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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったらfalseになる
			
			if (clearnness)
			{
				/*if (m_pPlayer->ChangeVel().y < 0.0f)
				{
					return false;
				}
				*/
			}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;
}
bool Bg::IsCollision1_2(Rect rect, Rect& chipRect)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ3番は当たり判定がないため飛ばす

			if (m_chipData0[x][y] == 3) continue;

			int chipLeft = static_cast<int>(x * kChipSize2 * kChipScale2);
			int chipRight = static_cast<int>(chipLeft + kChipSize2 * kChipScale2);
			int chipTop = static_cast<int>(y * kChipSize2 * kChipScale2);
			int chipBottom = static_cast<int>(chipTop + kChipSize2 * kChipScale2);

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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったらfalseになる
			//bool clearnness = (m_chipData0[x][y] == 38 ||
			//				   m_chipData0[x][y] == 39 ||
			//				   m_chipData0[x][y] == 40);
			//if (clearnness)
			//{
			//	/*if (m_pPlayer->ChangeVel().y < 0.0f)
			//	{
			//		return false;
			//	}
			//	*/
			//}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;
}
bool Bg::IsCollision1_3(Rect rect, Rect& chipRect)
{

	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 5) continue;
			if (m_chipData0[x][y] == 0) continue;

			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			int chipLeft;
			int chipRight;
			int chipTop;
			int chipBottom;

			if (clearnness)
			{
				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale * kThrough);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			}
			else
			{

				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);
			}

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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったらfalseになる
		
			if (clearnness)
			{
				/*if (m_pPlayer->ChangeVel().y < 0.0f)
				{
					return false;
				}
				*/
			}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;
}
bool Bg::IsCollisionClear(Rect rect, Rect& chipRect)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 5) continue;
			if (m_chipData0[x][y] == 0) continue;


			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			int chipLeft;
			int chipRight;
			int chipTop;
			int chipBottom;

			if (clearnness)
			{
				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale * kThrough);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			}
			else
			{

				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);
			}
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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったらfalseになる

			if (clearnness)
			{
				/*if (m_pPlayer->ChangeVel().y < 0.0f)
				{
					return false;
				}
				*/
			}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;
}
bool Bg::IsCollisionPlayer(Rect rect, Rect& chipRect,bool& hitClearness)
{
	bool ans;
	//マップごとに判定を変える
	switch (StageNum)
	{
	case 0:
		ans = IsCollisionPlayer1_1(rect, chipRect,hitClearness);
		break;
	case 1://1_1のマップ
		ans = IsCollisionPlayer1_1(rect, chipRect,hitClearness);
		break;
	case 2://1_2のマップ
		ans = IsCollisionPlayer1_2(rect, chipRect, hitClearness);
		break;
	case 3://1_3のマップ
		ans = IsCollisionPlayer1_3(rect, chipRect, hitClearness);
		break;
	case 4://stageClearのマップ
		ans = IsCollisionPlayer1_1(rect, chipRect, hitClearness);
		break;
	case 6://2_1のマップ
		ans = IsCollisionPlayer1_3(rect, chipRect, hitClearness);
		break;
	case 7://2_2のマップ
		ans = IsCollisionPlayer1_3(rect, chipRect, hitClearness);
		break;
	case 8://2_3のマップ
		ans = IsCollisionPlayer1_3(rect, chipRect, hitClearness);
		break;
	case 11://2_3のマップ
		ans = IsCollisionPlayer1_3(rect, chipRect, hitClearness);
		break;
	default:
		assert(false && "ここはIsCollisionの判定を変えるところです");

		break;
	}
	return ans;
}

bool Bg::IsCollisionPlayer1_1(Rect rect, Rect& chipRect,bool& hitClearness)
{
	hitClearness = false;//最初に初期化
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 5) continue;
			if (m_chipData0[x][y] == 0) continue;

			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			int chipLeft;
			int chipRight;
			int chipTop;
			int chipBottom;

			if (clearnness)
			{
				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale * kThrough);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			}
			else
			{

				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);
			}

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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったら
			// falseになる
			
			if (clearnness)
			{
				if (m_pPlayer->ChangeVel().y < 0.0f)
				{
					return false;
				}
				hitClearness = true;

			}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;

}

bool Bg::IsCollisionPlayer1_2(Rect rect, Rect& chipRect, bool& hitClearness)
{
	hitClearness = false;//最初に初期化
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ3番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 3) continue;

			int chipLeft = static_cast<int>(x * kChipSize2 * kChipScale2);
			int chipRight = static_cast<int>(chipLeft + kChipSize2 * kChipScale2);
			int chipTop = static_cast<int>(y * kChipSize2 * kChipScale2);
			int chipBottom = static_cast<int>(chipTop + kChipSize2 * kChipScale2);

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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			////プレイヤーの速度が上向きだったらfalseになる
			//bool clearnness = (m_chipData0[x][y] == 39 ||
			//				   m_chipData0[x][y] == 40 ||
			//				   m_chipData0[x][y] == 41);
			//if (clearnness)
			//{
			//	if (m_pPlayer->ChangeVel().y < 0.0f)
			//	{
			//		return false;
			//	}
			//hitClearness = true;
			//}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;

}

bool Bg::IsCollisionPlayer1_3(Rect rect, Rect& chipRect, bool& hitClearness)
{
	hitClearness = false;//最初に初期化
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップ0番は当たり判定がないため飛ばす
			if (m_chipData0[x][y] == 5) continue;
			if (m_chipData0[x][y] == 0) continue;

			bool clearnness = (m_chipData0[x][y] == 39 ||
							   m_chipData0[x][y] == 40 ||
							   m_chipData0[x][y] == 41);
			int chipLeft;
			int chipRight;
			int chipTop;
			int chipBottom;

			if (clearnness)
			{
				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale * kThrough);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			}
			else
			{

				chipLeft = static_cast<int>(x * kChipSize * kChipScale);
				chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
				chipTop = static_cast<int>(y * kChipSize * kChipScale);
				chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);
			}

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

			//マップチップが39,40,41のとき下からあたっていたら貫通したいから
			//プレイヤーの速度が上向きだったらfalseになる
		
			if (clearnness)
			{
				if (m_pPlayer->ChangeVel().y < 0.0f)
				{
					return false;
				}
				hitClearness = true;
			}

			// いずれかのチップに当たっていたら終了する
			return true;
		}
	}
	return false;
}

