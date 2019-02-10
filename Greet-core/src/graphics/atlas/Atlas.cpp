#include "Atlas.h"

namespace Greet {

  Atlas::Atlas(uint atlasSize, uint textureSize)
    : Texture2D(atlasSize,atlasSize,TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::RGBA)), m_textureSize(textureSize)
  {
    ASSERT(atlasSize > m_textureSize, "ATLAS", "Atlas size must be greater than the textures sizes");
    //ASSERT(!(atlasSize == 0) && !(atlasSize & (atlasSize - 1)),"ATLAS", "Atlas size must be a power of two");
    //ASSERT(!(textureSize == 0) && !(m_textureSize & (m_textureSize - 1)), "ATLAS", "Texture size must be a power of two");
    m_texturesSide = atlasSize / m_textureSize;
    m_textures = (atlasSize / m_textureSize)*(atlasSize / m_textureSize);
    uint bitCount = atlasSize * atlasSize * 4;

    occupied.resize(m_textures);
    std::fill(occupied.begin(), occupied.end(), false);

    BYTE* bits = new BYTE[bitCount];
    for (uint i = 0; i < bitCount; i+=4)
    {
      bits[i+FI_RGBA_RED  ]	 = 255;
      bits[i+FI_RGBA_GREEN]	 = 0;
      bits[i+FI_RGBA_BLUE]	 = 255;
      bits[i+FI_RGBA_ALPHA]	 = 255;
    }

    GenTexture(bits);
    delete[] bits;
  }

  Atlas::~Atlas()
  {
  }

  void Atlas::GenTexture(BYTE* bits)
  {
    GLCall(glBindTexture(GL_TEXTURE_2D, *texId));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  }

  bool Atlas::AddTexture(const std::string& name, const std::string& filePath)
  {
    uint width;
    uint height;
    BYTE* bits = ImageUtils::loadImage(filePath.c_str(), &width, &height);
    if (width != m_textureSize || height != m_textureSize)
    {
      Log::Error("The given textures size is not valid: ",name.c_str()," (",width,",",height,")");
      return false;
    }
    bool success = AddTexture(bits,name);

    delete[] bits;

    return success;
  }

  bool Atlas::AddTexture(BYTE* bits, const std::string& name)
  {
    uint textures = m_width / m_textureSize;
    if (textureMap.size() >= textures*textures)
    {
      Log::Error("There is no more room in the Atlas. Increase size or create a new one.");
      return false;
    }
    uint x = m_texturesSide;
    uint y = m_texturesSide;

    for (uint i = 0; i < m_textures; i++)
    {
      if (!occupied[i])
      {
        textureMap.emplace(name,i);
        occupied[i] = true;
        x = i % m_texturesSide;
        y = (i - x) / m_texturesSide;
        GLCall(glBindTexture(GL_TEXTURE_2D, *texId));
        GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x*m_textureSize,m_textureSize*m_texturesSide -m_textureSize - y*m_textureSize,m_textureSize,m_textureSize,GL_RGBA, GL_UNSIGNED_BYTE, bits));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        return true;
      }
    }
    ASSERT(false, "AtlasManager failed to find unoccupied space, even though it passed a space check");
  }

  Sprite Atlas::GetSprite(const std::string& name) const
  {
    return GetSpriteFromSheet(name, Vec2(0, 0), Vec2(1, 1));
  }

  Sprite Atlas::GetSpriteFromSheet(const std::string& sheetName, Vec2 texPos, Vec2 texSize) const
  {
    auto it = textureMap.find(sheetName);
    if(it != textureMap.end())
    {
      uint j = it->second;
      uint x = j % (m_width / m_textureSize);
      uint y = (j - x) / (m_width / m_textureSize);
      float size = (float)m_textureSize / (float)m_width;
      Vec2 spriteSize = Vec2(size,size);
      Vec2 spritePos = spriteSize*Vec2(x, y);
      spritePos += texPos * spriteSize;
      spriteSize *= texSize;
      return Sprite(*this, spritePos, spriteSize);
    }

    Log::Error("No texture found in Atlas: (", sheetName.c_str(), ")");
    return Sprite(*this);
  }

  void Atlas::RemoveTexture(const std::string& textureName)
  {
    auto it = textureMap.find(textureName);
    if(it != textureMap.end())
    {
      occupied[it->second] = false;
      textureMap.erase(it);
    }
  }
}
