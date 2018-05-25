#pragma once

#include <vector>
#include <graphics/atlas/Atlas.h>
#include <logging/Log.h>
#include <internal/GreetTypes.h>
#include <utils/ErrorHandle.h>

namespace Greet {
	class AtlasManager
	{
	private:
		static std::vector<Atlas*> m_atlas;
	public:
		static void Add(Atlas* atlas);
		static Atlas* Get(const std::string& atlasname);
		static void Destroy();
	private:
		AtlasManager(){}
	};
}
