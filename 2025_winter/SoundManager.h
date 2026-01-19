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

	void SetMasterVolume(int volume);//0～255

private:
	std::unordered_map<std::string, int> m_sounds;
	int m_masterVolume = 100;//255が最大
	int m_masterVolume2 = 200;//255が最大
	int m_currentBgm = -1;
	
	//3重再生用スロット
	std::vector<SeSlot> m_seSlots;
};


