#include "SoundManager.h"
#include "DxLib.h"
#include <cassert>



void SoundManager::Init()
{
	//BGmの読み込み
    m_sounds["bgm"] = LoadSoundMem("data/Sound/TitleSceneBgm.mp3");
    m_sounds["bgmSelectScene"] = LoadSoundMem("data/Sound/StageSelectBgm.ogg");
    m_sounds["bgmStageScene"] = LoadSoundMem("data/Sound/stageBgm.ogg");
    m_sounds["bgmClearScene"] = LoadSoundMem("data/Sound/StageClearBgm.ogg");
    m_sounds["bgmGameOverScene"] = LoadSoundMem("data/Sound/GameOverBgm.ogg");
	assert(m_sounds["bgm"] >= 0);

	//SEの読み込み
	//環境音
	m_sounds["ok"] = LoadSoundMem("data/Sound/SE/ok.mp3");
	m_sounds["openDoor"] = LoadSoundMem("data/Sound/SE/openDoor.mp3");
	m_sounds["iceMove"] = LoadSoundMem("data/Sound/SE/IceMove.mp3");
	m_sounds["cursor"] = LoadSoundMem("data/Sound/SE/cursor.mp3");
	m_sounds["cursorError"] = LoadSoundMem("data/Sound/SE/cursorError.mp3");
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

	// サウンド名のリスト
    const std::vector<std::string> soundNames = {
        "bgm",
        "bgmSelectScene",
        "bgmStageScene",
        "bgmClearScene",
        "bgmGameOverScene",
    };
	//SEのリスト
    const std::vector<std::string> seNames = {
        "ok",
        "airSlash",
		"iceMove",
		"cursor",
		"cursorError",
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
        "yarareSE"
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

void SoundManager::SetMasterVolume(int volume)
{
    m_masterVolume = volume;
}

