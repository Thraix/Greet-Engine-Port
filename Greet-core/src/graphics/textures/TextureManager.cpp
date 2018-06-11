#include "TextureManager.h"

namespace Greet{

	std::map<std::string, Texture> TextureManager::m_textures;

	void TextureManager::Add(const std::string& name, const Texture& texture)
	{
    if(m_textures.find(name) != m_textures.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
      Log::Error("Given texture name already exists: ", name);
      return;
    }
		m_textures.emplace(name,texture);
	}

	const Texture& TextureManager::Get(const std::string& name)
	{
    auto it = m_textures.find(name);
    if(it == m_textures.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
      Log::Error("Could not find the given texture: ", name);
      return m_textures.begin()->second;
    } 
    return it->second;
	}

	const Texture2D& TextureManager::Get2D(const std::string& texturename)
	{
		return (const Texture2D&)Get(texturename);
	}

	const CubeMap& TextureManager::Get3D(const std::string& texturename)
	{
		return (const CubeMap&)Get(texturename);
	}

	void TextureManager::Destroy()
	{
    m_textures.clear();
	}

}
