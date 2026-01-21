#pragma once
#include <map>
#include <string>
class EffekseerResourceManager
{
	struct  EffekseerResourceInfo
	{
		int refCounter = 0;
		int handle;
	};
	std::map<std::string, EffekseerResourceInfo> m_effectResources;
public:
	EffekseerResourceManager();
	int LoadEffect(const std::string& path);
	int DeleteEffect(int handle);
};

