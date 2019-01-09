#pragma once

#include <vector>
#include <graphics/atlas/Atlas.h>

namespace Greet {
  class AtlasManager
  {
    private:
      static std::map<std::string, Atlas> m_atlas;
    public:
      static void Add(const std::string& name, Atlas&& atlas);
      static const Atlas& Get(const std::string& atlasname);
      static void Destroy();
    private:
      AtlasManager(){}
  };
}
