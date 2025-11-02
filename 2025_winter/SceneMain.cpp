#include "SceneMain.h"
#include "Player.h"
SceneMain::SceneMain()
{
	m_pPlayer = new Player;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{

}
void SceneMain::Update()
{
	m_pPlayer->Update();
}
void SceneMain::Draw()
{
	m_pPlayer->Draw();

}
