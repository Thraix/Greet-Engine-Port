#include "TextureManager.h"

namespace Greet{

	std::vector<Texture*> TextureManager::m_textures;

	void TextureManager::Add(Texture* texture)
	{
		for (int i = 0;i < m_textures.size();i++)
		{
			if (texture->GetName() == m_textures[i]->GetName())
			{
				ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
				Log::Error("Given texture name already exists: ", texture->GetName().c_str());
				return;
			}
		}
		m_textures.push_back(texture);
	}

	Texture* TextureManager::Get(const std::string& texturename)
	{
		uint size = m_textures.size();
		for (uint i = 0; i < size; i++)
		{
			if (texturename.compare(m_textures[i]->GetName().c_str()) == 0)
			{
				return m_textures[i];
			}
		}
		ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
		Log::Error("Could not find the given texture: ", texturename.c_str());
		return m_textures[0];
	}

	Texture2D* TextureManager::Get2D(const std::string& texturename)
	{
		return (Texture2D*)Get(texturename);
	}

	CubeMap* TextureManager::Get3D(const std::string& texturename)
	{
		return (CubeMap*)Get(texturename);
	}

	void TextureManager::Destroy()
	{
		for (uint i = 0; i < m_textures.size(); i++)
		{
			delete m_textures[i];
		}
	}

}