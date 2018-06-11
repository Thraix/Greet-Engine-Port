#pragma once


#include <vector>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/CubeMap.h>

namespace Greet {
	class TextureManager
	{
	private:
		static std::map<std::string, Texture> m_textures;
	public:

		static void Add(const std::string& name, const Texture& texture);
		static const Texture& Get(const std::string& texturename);
		static const Texture2D& Get2D(const std::string& texturename);
		static const CubeMap& Get3D(const std::string& texturename);
		static void Destroy();
	private:
		TextureManager(){}
	};
}
