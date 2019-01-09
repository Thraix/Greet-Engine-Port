#pragma once

#include <utils/ImageUtils.h>
#include <graphics/textures/Texture.h>
#include <graphics/Sprite.h>
#include <list>
#include <vector>

namespace Greet{
  class Atlas : public Texture2D
  {
    private:
      std::vector<std::string> m_textureNames;
      std::vector<uint> m_textureNamePos;
      std::vector<bool> m_occupied;

      uint m_textureSize;
      uint m_texturesSide;
      uint m_textures;

      BYTE* m_bits;
    public:
      Atlas(uint atlasSize, uint textureSize);
      Atlas(Atlas&&) = default;
      Atlas& operator=(Atlas&&) = default;
      virtual ~Atlas();

      bool AddTexture(std::string name, std::string filePath);
      void RemoveTexture(std::string textureName);
      void AddTexture(BYTE* bits, uint bpp, std::string name);
      Sprite GetSprite(std::string name) const;
      Sprite GetSpriteFromSheet(std::string sheetName, Vec2 texPos, Vec2 texSize) const;
    private:
      void GenTexture(BYTE* bits);
  };
}
