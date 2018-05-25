#pragma once

#include <logging/Log.h>
#include <vector>
#include "Texture.h"
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/CubeMap.h>
#include <utils/ErrorHandle.h>

namespace Greet {
	class TextureManager
	{
	private:
		static std::vector<Texture*> m_textures;
	public:

		static void Add(Texture* texture);
		static Texture* Get(const std::string& texturename);
		static Texture2D* Get2D(const std::string& texturename);
		static CubeMap* Get3D(const std::string& texturename);
		static void Destroy();
	private:
		TextureManager(){}
	};
}