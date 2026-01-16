#pragma once
#include <string>
#include <unordered_map>
class SoundManager
{
public:

	void Init();
	void Update();

	void PlaySE(const std::string& name);
	void PlayBgm(const std::string& name);

	void StopBgm();

	void SetMasterVolume(int volume);//0～255

private:
	std::unordered_map<std::string, int> m_sounds;
	int m_masterVolume =128;//255が最大
	int m_currentBgm = -1;
	
};

