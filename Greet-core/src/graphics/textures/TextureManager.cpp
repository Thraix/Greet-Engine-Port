#include "TextureManager.h"

#include <utils/ErrorHandle.h>

namespace Greet{

  std::map<std::string, CubeMap> TextureManager::m_cubeMaps;
  std::map<std::string, Texture2D> TextureManager::m_texture2Ds;

  Texture2D TextureManager::emptyTexture2D{};
  CubeMap TextureManager::emptyCubeMap{};

  const Texture2D& TextureManager::Add(const std::string& name, Texture2D&& texture)
  {
    auto it = m_texture2Ds.find(name);
    if(it != m_texture2Ds.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
      Log::Error("Given texture name already exists: ", name);
      return it->second;
    }
    return m_texture2Ds.emplace(name,std::move(texture)).first->second;
  }

  const CubeMap& TextureManager::Add(const std::string& name, CubeMap&& texture)
  {
    auto it = m_cubeMaps.find(name);
    if(it != m_cubeMaps.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
      Log::Error("Given texture name already exists: ", name);
      return it->second;
    }
    return m_cubeMaps.emplace(name,std::move(texture)).first->second;
  }

  const Texture2D& TextureManager::Get2D(const std::string& texturename)
  {
    auto it = m_texture2Ds.find(texturename);
    if(it == m_texture2Ds.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
      Log::Error("Could not find the given texture: ", texturename);
      return emptyTexture2D;
    } 
    return it->second;
  }

  const CubeMap& TextureManager::Get3D(const std::string& texturename)
  {
    auto it = m_cubeMaps.find(texturename);
    if(it == m_cubeMaps.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
      Log::Error("Could not find the given texture: ", texturename);
      return emptyCubeMap;
    } 
    return it->second;
  }

  const Texture2D& TextureManager::GetEmptyTexture2D()
  {
    return emptyTexture2D;
  }

  const CubeMap& TextureManager::GetEmptyCubeMap()
  {
    return emptyCubeMap;
  }

  void TextureManager::Destroy()
  {
    m_texture2Ds.clear();
    m_cubeMaps.clear();
  }

}
