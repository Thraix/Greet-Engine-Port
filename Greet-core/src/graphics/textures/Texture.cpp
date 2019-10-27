#include "Texture.h"

#include <internal/GreetGL.h>

namespace Greet {

  void TextureDeleter::operator()(uint* id)
  {
    glDeleteTextures(1, id);
    delete id;
  }

  Texture::Texture(uint textureType, bool generateTexture)
    : texId{new uint{0}}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum: ", (uint)m_textureType);
    if(generateTexture)
    {
      uint id;
      glGenTextures(1, &id);
      *texId = id;
    }
    else
    {
      *texId = 0;
    }
  }

  Texture::Texture(uint texId, uint textureType)
    : texId{new uint{texId}}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum", (uint)m_textureType);
  }

  Texture::Texture()
    : texId{new uint{0}}, m_textureType{0}
  {
  }

  void Texture::Enable() const
  {
    GLCall(glBindTexture(m_textureType, *texId));
  }

  void Texture::Enable(uint index) const
  {
    GLCall(glActiveTexture(GL_TEXTURE0 + index));
    GLCall(glBindTexture(m_textureType, *texId));
  }

  void Texture::Disable() const
  {
    GLCall(glBindTexture(m_textureType, 0));
  }

  uint Texture::GetTexId() const
  {
    return *texId.get();
  }
}
