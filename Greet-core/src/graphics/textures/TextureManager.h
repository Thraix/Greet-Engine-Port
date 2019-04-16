#pragma once

#include <map>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/CubeMap.h>

namespace Greet {
  class TextureManager
  {
    private:
      static std::map<std::string, CubeMap> m_cubeMaps;
      static std::map<std::string, Texture2D> m_texture2Ds;
      static Texture2D emptyTexture2D;
      static CubeMap emptyCubeMap;
    public:

      static const Texture2D& Add(const std::string& name, Texture2D&& texture);
      static const CubeMap&  Add(const std::string& name, CubeMap&& texture);
      static const Texture& Get(const std::string& texturename);
      static const Texture2D& Get2D(const std::string& texturename);
      static const CubeMap& Get3D(const std::string& texturename);
      // These are basically textures that cannot be renderered. texId = 0
      static const Texture2D& GetEmptyTexture2D();
      static const CubeMap& GetEmptyCubeMap();
      static void Destroy();
    private:
      TextureManager(){}
  };
}
