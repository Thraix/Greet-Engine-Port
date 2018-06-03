#include "AtlasManager.h"	

namespace Greet{
	std::vector<Atlas*> AtlasManager::m_atlas;

	void AtlasManager::Add(Atlas* atlas)
	{
		for (size_t i = 0;i < m_atlas.size();i++)
		{
			if (atlas->GetName() == m_atlas[i]->GetName())
			{
				ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
				Log::Error("Given atlas name already exists: ", atlas->GetName().c_str());
				return;
			}
		}
		m_atlas.push_back(atlas);
	}

	Atlas* AtlasManager::Get(const std::string& atlasName)
	{
		uint size = m_atlas.size();
		for (uint i = 0; i < size; i++)
		{
			if (atlasName.compare(m_atlas[i]->GetName().c_str()) == 0)
			{
				return m_atlas[i];
			}
		}
		ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
		Log::Error("Could not find the given atlas: ", atlasName.c_str());
		return m_atlas[0];
	}

	void AtlasManager::Destroy()
	{
		for (uint i = 0; i < m_atlas.size(); i++)
			delete m_atlas[i];
	}

}
