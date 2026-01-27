#pragma once
#include <string>
#include <unordered_map>


struct SeSlot
{
	int handle;
};

class SoundManager
{
public:

	void Init();
	void Update();

	void PlaySE(const std::string& name);
	void PlayBgm(const std::string& name);

	void StopBgm();
	void StopSE(const std::string& name);
	//セット
	void SetBgmVolume(int volume);//0～255
	void SetSEVolume(int volume);//0～255
	//音量設定用
	int GetBgmVolume() { return m_displayBgm; }
	int GetSEVolume() { return m_displaySE; }

private:
	std::unordered_map<std::string, int> m_sounds;
	//内部的なもの
	int m_masterVolume = 128;//255が最大
	int m_masterVolume2 = 128;//255が最大
	//見た目用
	int m_displayBgm = 50;//100が最大
	int m_displaySE = 50;//100が最大
	int m_currentBgm = -1;
	
	//3重再生用スロット
	std::vector<SeSlot> m_seSlots;
};


