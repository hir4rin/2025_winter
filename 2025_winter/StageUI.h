#pragma once
#include "Player.h"
class Camera;
class SceneController;



class StageUI
{
public:
	StageUI(SceneController& controller);
	virtual ~StageUI();

	void Init(int hp, PlayerType type, int Life);
	void Update();
	void Draw(Camera& camera);
	/// <summary>
	/// プレイヤーのHPをセットする(UI用)
	/// </summary>
	/// <param name="m_pPlayerHP"></param>
	void SetHp(int  m_pPlayerHP);
    /// <summary>
    /// プレイヤーのTypeをセットする(UI用)
    /// </summary>
    /// <param name="m_type"></param>
    void SetType(PlayerType m_type);
	/// <summary>
	/// プレイヤーのlifeをセットする
	/// </summary>
	/// <param name="m_pPlayerLife"></param>
	void SetLife(int m_pPlayerLife);


	/// <summary>
	/// ボスのHPをセットする(UI用)
	/// </summary>
	/// <param name="m_pBossHp"></param>
	void SetBossHp(int m_pBossHp);
	/// <summary>
	/// ボス魚のHPをセットする(UI用)
	/// </summary>
	/// <param name="m_pBossFishHp"></param>
	void SetFishHp(int m_pBossFishHp);
	/// <summary>
	/// 熊と狼のhpをセットする(UI用)
	/// </summary>
	/// <param name="bearHp"></param>
	/// <param name="WolfHp"></param>
	void SetBearWolfHp(int bearHp, int WolfHp);

	float GetTopX();

	void CopyDesign();

private:
	int m_hpHandle;
	int m_playerHp;

	int m_playerLife;

	int m_bossHpHandle;
	int m_bossHp;

	int  m_bossFishHandle;
	int m_bossFishHp;

	int m_BearWolfHandle;
	int m_BearWolfHp; //熊と狼

	//表示用アニメーション
	float m_displayHp;
	PlayerType m_displayType;
	//ボス
	float m_displayBossHp;
	//魚
	float m_displayBossFishHp;
	//熊と狼
	float m_displayBearWolfhp;


private:
	int m_frame;
	int m_normalHandle;
	int m_burningHandle;
	int m_frozenHandle;
	int m_archerHandle;
	std::shared_ptr<Player> m_pPlayer;
	//int m_playerHandle;


private:
	int m_fontHandle;// フォントハンドル

};

