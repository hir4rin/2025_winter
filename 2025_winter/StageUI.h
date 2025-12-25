#pragma once
#include "Player.h"
class Camera;



class StageUI
{
public:
	StageUI();
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

	float GetTopX();

	void CopyDesign();

private:
	int m_hpHandle;
	int m_playerHp;

	int m_playerLife;

	int m_bossHpHandle;
	int m_bossHp;
	//表示用アニメーション
	float m_displayHp;
	PlayerType m_displayType;

	float m_displayBossHp;
	

};

