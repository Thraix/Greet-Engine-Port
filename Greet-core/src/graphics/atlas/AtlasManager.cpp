#include "AtlasManager.h"	

#include <logging/Log.h>
#include <internal/GreetTypes.h>
#include <utils/ErrorHandle.h>

namespace Greet{
  std::map<std::string, const Atlas&> AtlasManager::m_atlas;

  void AtlasManager::Add(const std::string& name, const Atlas& atlas)
  {
    if(m_atlas.find(name) != m_atlas.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
      Log::Error("Given atlas name already exists: ", name);
      return;
    }
    m_atlas.emplace(name, atlas);
  }

  const Atlas& AtlasManager::Get(const std::string& name)
  {
    auto it = m_atlas.find(name);
    if(it == m_atlas.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
      Log::Error("Could not find the given atlas: ", name);
      return m_atlas.begin()->second; 
    }
    return it->second;
  }

  void AtlasManager::Destroy()
  {
    m_atlas.clear();
  }

}
