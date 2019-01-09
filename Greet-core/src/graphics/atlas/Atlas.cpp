#include "Atlas.h"

namespace Greet {

  Atlas::Atlas(uint atlasSize, uint textureSize)
    : Texture2D(atlasSize,atlasSize,TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::RGBA,TextureFormat::INVERTED)), m_textureSize(textureSize)
  {
    ASSERT(atlasSize > m_textureSize, "ATLAS", "Atlas size must be greater than the textures sizes");
    //ASSERT(!(atlasSize == 0) && !(atlasSize & (atlasSize - 1)),"ATLAS", "Atlas size must be a power of two");
    //ASSERT(!(textureSize == 0) && !(m_textureSize & (m_textureSize - 1)), "ATLAS", "Texture size must be a power of two");
    m_texturesSide = atlasSize / m_textureSize;
    m_textures = (atlasSize / m_textureSize)*(atlasSize / m_textureSize);
    uint bitCount = atlasSize * atlasSize * 4;

    BYTE* bits = new BYTE[bitCount];
    for (uint i = 0; i < bitCount; i+=4)
    {
      bits[i+FI_RGBA_RED  ]	 = 255;
      bits[i+FI_RGBA_GREEN]	 = 0;
      bits[i+FI_RGBA_BLUE]	 = 255;
      bits[i+FI_RGBA_ALPHA]	 = 255;
    }
    for (uint i = 0; i < m_textures; i++)
    {
      m_occupied.push_back(false);
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

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  }

  bool Atlas::AddTexture(std::string name, std::string filePath)
  {
    uint textures = m_width / m_textureSize;
    if (m_textureNames.size() >= textures*textures)
    {
      Log::Error("There is no more room in the Atlas. Increase size or create a new one.");
      return false;
    }
    uint width;
    uint height;
    uint bpp;
    BYTE* bits = ImageUtils::loadImage(filePath.c_str(), &width, &height,&bpp);
    if (width != m_textureSize || height != m_textureSize)
    {
      Log::Error("The given textures size is not valid: ",name.c_str()," (",width,",",height,")");
      return false;
    }
    AddTexture(bits,bpp,name);

    delete[] bits;

    return true;
  }

  void Atlas::AddTexture(BYTE* bits, uint bpp, std::string name)
  {
    uint x = m_texturesSide;
    uint y = m_texturesSide;
    for (uint i = 0; i < m_textures; i++)
    {
      if (!m_occupied[i])
      {
        x = i%m_texturesSide;
        y = (i - x) / m_texturesSide;
        m_textureNames.push_back(name);
        m_textureNamePos.push_back(i);
        m_occupied[i] = true;
        break;
      }
    }
    if (x == m_texturesSide || y == m_texturesSide)
    {
      Log::Error("There is no more room in the Atlas. Increase size or create a new one.");
      return;
    }

    GLCall(glBindTexture(GL_TEXTURE_2D, *texId));
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x*m_textureSize,m_textureSize*m_texturesSide -m_textureSize - y*m_textureSize,m_textureSize,m_textureSize,bpp == 32 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, bits));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  }

  Sprite Atlas::GetSprite(std::string name) const
  {
    return GetSpriteFromSheet(name, Vec2(0, 0), Vec2(1, 1));
  }

  Sprite Atlas::GetSpriteFromSheet(std::string sheetName, Vec2 texPos, Vec2 texSize) const
  {
    uint size = m_textureNames.size();
    for (uint i = 0; i < size; i++)
    {
      if (m_textureNames[i].compare(sheetName)==0)
      {
        uint j = m_textureNamePos[i];
        uint x = j % (m_width / m_textureSize);
        uint y = (j - x) / (m_width / m_textureSize);
        float size = (float)m_textureSize / (float)m_width;
        Vec2 spriteSize = Vec2(size,size);
        Vec2 spritePos = spriteSize*Vec2(x, y);
        spritePos += texPos * spriteSize;
        spriteSize *= texSize;
        return Sprite(*this, spritePos, spriteSize);
      }
    }
    Log::Error("No texture found in Atlas: (", sheetName.c_str(), ")");
    return Sprite(*this);
  }

  void Atlas::RemoveTexture(std::string textureName)
  {
    uint size = m_textureNames.size();
    for (uint i = 0; i < size; i++)
    {
      if (m_textureNames[i].compare(textureName) == 0)
      {
        m_textureNames.erase(m_textureNames.begin() + i);
        m_textureNamePos.erase(m_textureNamePos.begin() + i);
        break;
      }
    }
  }
}
