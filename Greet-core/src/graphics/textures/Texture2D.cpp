#include "Texture2D.h"

#include <internal/GreetGL.h>

namespace Greet {

  Texture2D::Texture2D(uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D), m_params(params)
  {
    GenTexture(width,height);
  }

  Texture2D::Texture2D(const std::string& filename, TextureParams params)
    :Texture(GL_TEXTURE_2D), m_params(params)
  {
    LoadTexture(filename);
  }

  Texture2D::Texture2D(BYTE* bits, uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D), m_width(width), m_height(height), m_params(params)
  {
    GenTexture(bits);
    delete[] bits;
  }

  Texture2D::Texture2D(TextureParams params)
    : Texture(GL_TEXTURE_2D,false), m_params(params)
  {

  }

  Texture2D::Texture2D(uint texId, uint width, uint height, TextureParams params)
    : Texture(texId, (uint)GL_TEXTURE_2D), m_width(width), m_height(height), m_params(params)
  {

  }

  Texture2D::~Texture2D()
  {

  }

  void Texture2D::LoadTexture(const std::string& filename)
  {
    BYTE* bits = ImageUtils::loadImage(filename.c_str(),&m_width,&m_height);
    GenTexture(bits);
    delete[] bits;
  }

  void Texture2D::GenTexture(uint width, uint height)
  {
    m_width = width;
    m_height = height;
    GenTexture(NULL);
  }

  void Texture2D::GenTexture(BYTE* bits)
  {
    Enable();
    if(m_params.filter != TextureFilter::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  (uint)m_params.filter));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint)m_params.filter));
    }
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)m_params.internalFormat, m_width, m_height, 0, (uint)m_params.internalFormat, GL_UNSIGNED_BYTE, bits));
    Disable();
  }

  void Texture2D::SetPixels(const void* pixels) const
  {
    Enable();
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, (uint)m_params.internalFormat,GL_UNSIGNED_BYTE, pixels));
  }
}
