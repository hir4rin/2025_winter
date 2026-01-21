#include "EffekseerResourceManager.h"
#include "EffekseerForDXLib.h"

EffekseerResourceManager::EffekseerResourceManager()
{
	
}

int EffekseerResourceManager::LoadEffect(const std::string& path)
{
	if(m_effectResources.contains(path))
	{
		auto& ref = m_effectResources[path];
		++ref.refCounter;
		return ref.handle;
	}
	else
	{
		int handle = LoadEffekseerEffect(path.c_str());
		m_effectResources[path].handle = handle;
		m_effectResources[path].refCounter = 1;
		return handle;
	}
}

int EffekseerResourceManager::DeleteEffect(int handle)
{
	for (auto& res : m_effectResources)
	{
		if (handle == res.second.handle)
		{
			if (--res.second.refCounter <= 0)
			{
				return DeleteEffekseerEffect(res.second.handle);
			}
			return 1;//まだ消えない
		}
	}
}
