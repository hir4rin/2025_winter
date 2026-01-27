#include "SoundManager.h"
#include "DxLib.h"
#include <cassert>


namespace
{
    //音量変換
    constexpr int kVolumeMax = 255;
    constexpr int kDisplayMax = 100;

}

void SoundManager::Init()
{
	//BGmの読み込み
    m_sounds["bgm"] = LoadSoundMem("data/Sound/TitleSceneBgm.mp3");
    m_sounds["bgmSelectScene"] = LoadSoundMem("data/Sound/StageSelectBgm.ogg");
    m_sounds["bgmStageScene"] = LoadSoundMem("data/Sound/stageBgm.ogg");
    m_sounds["bgmClearScene"] = LoadSoundMem("data/Sound/StageClear.mp3");
    m_sounds["bgmGameOverScene"] = LoadSoundMem("data/Sound/GameOverBgm.ogg");
    m_sounds["bgmWeaponSelectScene"] = LoadSoundMem("data/Sound/WeaponSelectSceneBgm.ogg");
    m_sounds["bgmElite"] = LoadSoundMem("data/Sound/EliteBossBgm.ogg");
    m_sounds["bgmFishBefore"] = LoadSoundMem("data/Sound/FishBefore.ogg");
    m_sounds["bgmFishAfter"] = LoadSoundMem("data/Sound/FishAfter.ogg");
    m_sounds["bgmBearWolf"] = LoadSoundMem("data/Sound/BearWolfBgm.ogg");
	assert(m_sounds["bgm"] >= 0);

	//SEの読み込み
	//環境音
	m_sounds["ok"] = LoadSoundMem("data/Sound/SE/ok.mp3");
	m_sounds["ok2"] = LoadSoundMem("data/Sound/SE/ok2.mp3");
	m_sounds["openDoor"] = LoadSoundMem("data/Sound/SE/openDoor.mp3");
	m_sounds["iceMove"] = LoadSoundMem("data/Sound/SE/IceMove.wav");
	m_sounds["cursor"] = LoadSoundMem("data/Sound/SE/cursor.mp3");
	m_sounds["cursorError"] = LoadSoundMem("data/Sound/SE/cursorError.mp3");
	m_sounds["bom"] = LoadSoundMem("data/Sound/SE/Bom.mp3");
	m_sounds["clear"] = LoadSoundMem("data/Sound/SE/clear.mp3");
	m_sounds["pauseOP"] = LoadSoundMem("data/Sound/SE/pauseOP.mp3");
	m_sounds["pauseCl"] = LoadSoundMem("data/Sound/SE/pauseCl.mp3");
	//攻撃音
	m_sounds["airSlash"] = LoadSoundMem("data/Sound/SE/airSlash.mp3");
	m_sounds["burningSE"] = LoadSoundMem("data/Sound/SE/BurningSE.mp3");
	m_sounds["frozenSE"] = LoadSoundMem("data/Sound/SE/FrozenSE.mp3");
	m_sounds["shotArrow"] = LoadSoundMem("data/Sound/SE/shotArrow.mp3");
	m_sounds["hitArrow"] = LoadSoundMem("data/Sound/SE/hitArrow.mp3");
	m_sounds["copyAction"] = LoadSoundMem("data/Sound/SE/copyAction.mp3");
	m_sounds["copyOut"] = LoadSoundMem("data/Sound/SE/copyOut.mp3");
	m_sounds["hitOut"] = LoadSoundMem("data/Sound/SE/hitOut.mp3");
	m_sounds["hitSE"] = LoadSoundMem("data/Sound/SE/hitSE.mp3");
    //playerからでる音
	m_sounds["copySE"] = LoadSoundMem("data/Sound/SE/CopySE.mp3");
	m_sounds["jump"] = LoadSoundMem("data/Sound/SE/jump.mp3");
	m_sounds["playerHit"] = LoadSoundMem("data/Sound/SE/playerHit.mp3");
	m_sounds["yarareSE"] = LoadSoundMem("data/Sound/SE/yarareSE.mp3");
	m_sounds["heal"] = LoadSoundMem("data/Sound/SE/heal.wav");
    //敵の音
    m_sounds["setSword"] = LoadSoundMem("data/Sound/SE/SetSword.mp3");
    m_sounds["wolfSlash"] = LoadSoundMem("data/Sound/SE/wolfSlash.mp3");
    m_sounds["wolfAirSlash"] = LoadSoundMem("data/Sound/SE/wolfAirSlash.mp3");
    m_sounds["bearsplash"] = LoadSoundMem("data/Sound/SE/bearsplash.mp3");
    m_sounds["bossDown"] = LoadSoundMem("data/Sound/SE/bossDown.mp3");
    m_sounds["waveSE"] = LoadSoundMem("data/Sound/SE/waveSE.mp3");
    m_sounds["droppedSE"] = LoadSoundMem("data/Sound/SE/IceMove.mp3");
    m_sounds["hpStartSE"] = LoadSoundMem("data/Sound/SE/hpStartSE.mp3");

	// サウンド名のリスト
    const std::vector<std::string> soundNames = {
        "bgm",
        "bgmSelectScene",
        "bgmStageScene",
        "bgmClearScene",
        "bgmGameOverScene",
        "bgmWeaponSelectScene",
        "bgmElite",
        "bgmFishBefore",
        "bgmFishAfter",
        "bgmBearWolf"
    };
	//SEのリスト
    const std::vector<std::string> seNames = {
        "ok",
        "ok2",
        "airSlash",
		"iceMove",
		"cursor",
		"cursorError",
        "bom",
        "clear",
        "pauseOP",
        "pauseCl"
        "openDoor",
        "burningSE",
        "frozenSE",
        "shotArrow",
        "hitArrow",
        "copyAction",
        "copySE",
		"jump",
        "copyOut",
        "hitOut",
        "hitSE",
        "playerHit",
        "yarareSE",
        "heal",
        "setSword",
        "wolfSlash",
        "wolfAirSlash",
        "bearsplash",
        "bossDown",
        "waveSE",
        "droppedSE"

	};
	
	//BGmの音量設定
  // BGM音量
    for (const auto& key : soundNames)
    {
        ChangeVolumeSoundMem(m_masterVolume, m_sounds[key]);
    }
	//SEの音量設定
    for (const auto& key : seNames)
    {
        ChangeVolumeSoundMem(m_masterVolume2, m_sounds[key]);
    }
	//3重再生用スロットの初期化
    for (int i = 0; i < 3; ++i)
    {
        int h = LoadSoundMem("data/Sound/SE/shotArrow.mp3");
        m_seSlots.push_back({ h });
    }
    
}

void SoundManager::PlaySE(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) return;

    if(name == "shotArrow")
    {
        // 3重再生用スロットを探す
        for (auto& slot : m_seSlots)
        {
            if ( CheckSoundMem(slot.handle) == 0)
            {
              //二回目が鳴らない
                ChangeVolumeSoundMem(m_masterVolume2, slot.handle);
                PlaySoundMem(slot.handle, DX_PLAYTYPE_BACK);
                return;
            }
        }
        // 全てのスロットが再生中の場合、最初のスロットを強制的に再生
        auto& slot = m_seSlots[0];
        StopSoundMem(slot.handle);
        ChangeVolumeSoundMem(m_masterVolume2, slot.handle);
        PlaySoundMem(slot.handle, DX_PLAYTYPE_BACK);
        return;
	}
    else
    {
        ChangeVolumeSoundMem(m_masterVolume2, it->second);
        PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
    }


 
}

void SoundManager::PlayBgm(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) return;

    if (m_currentBgm != -1)
    {
        StopSoundMem(m_currentBgm);
    }

    m_currentBgm = it->second;//ハンドルの番号
    ChangeVolumeSoundMem(m_masterVolume, m_currentBgm);
    PlaySoundMem(m_currentBgm, DX_PLAYTYPE_LOOP);
}

void SoundManager::StopBgm()
{
    if (m_currentBgm != -1)
    {
        StopSoundMem(m_currentBgm);
        m_currentBgm = -1;
    }
}

void SoundManager::StopSE(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) return;
    StopSoundMem(it->second);
}

void SoundManager::SetBgmVolume(int volume)
{
    m_displayBgm = volume;
    int i = m_displayBgm * kVolumeMax / kDisplayMax;
    if (i >= 255)i = 255;
    m_masterVolume = i;
    ChangeVolumeSoundMem(m_masterVolume, m_currentBgm);
}

void SoundManager::SetSEVolume(int volume)
{
    m_displaySE = volume;
    int i = m_displaySE * kVolumeMax / kDisplayMax * 1.4f;//ちょっと大きくする
    if (i >= 255)i = 255;
    m_masterVolume2 = i;
    //SEを流す
    PlaySE("cursor");
}

