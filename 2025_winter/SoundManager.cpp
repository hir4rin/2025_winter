#include "SoundManager.h"
#include "DxLib.h"
#include <cassert>



void SoundManager::Init()
{
   // m_sounds["jump"] = LoadSoundMem("res/se/jump.wav");
    //m_sounds["hit"] = LoadSoundMem("res/se/hit.wav");
    m_sounds["bgm"] = LoadSoundMem("data/Sound/TitleSceneBgm.ogg");
    m_sounds["bgmSelectScene"] = LoadSoundMem("data/Sound/StageSelectBgm.ogg");
    m_sounds["bgmStageScene"] = LoadSoundMem("data/Sound/stageBgm.ogg");
    m_sounds["bgmClearScene"] = LoadSoundMem("data/Sound/StageClearBgm.ogg");
	assert(m_sounds["bgm"] >= 0);

    ChangeVolumeSoundMem(m_masterVolume, m_sounds["bgm"]);
    ChangeVolumeSoundMem(m_masterVolume, m_sounds["bgmSelectScene"]);
    ChangeVolumeSoundMem(m_masterVolume, m_sounds["bgmStageScene"]);
    ChangeVolumeSoundMem(m_masterVolume, m_sounds["bgmClearScene"]);

    
}

void SoundManager::PlaySE(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) return;

    ChangeVolumeSoundMem(m_masterVolume, it->second);
    PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
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

void SoundManager::SetMasterVolume(int volume)
{
    m_masterVolume = volume;
}

