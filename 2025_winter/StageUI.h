#pragma once
#include "Player.h"
class Camera;



class StageUI
{
public:
	StageUI();
	virtual ~StageUI();

	void Init();
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

	float GetTopX();

	void CopyDesign();

private:
	int m_hpHandle;
	int m_playerHp;
	//表示用アニメーション
	float m_displayHp;
	PlayerType m_displayType;
	

};

