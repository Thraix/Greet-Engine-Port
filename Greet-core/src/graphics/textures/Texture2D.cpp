#include "Texture2D.h"

#include <internal/GreetGL.h>

namespace Greet {

  Texture2D::Texture2D(uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D,true), m_params(params)
  {
    GenTexture(width,height);
  }

  Texture2D::Texture2D(const std::string& filename, TextureParams params)
    : Resource(filename), Texture(GL_TEXTURE_2D,true), m_params(params)
  {
    LoadTexture(filename);
  }

  Texture2D::Texture2D(const std::vector<byte>& bits, uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D,true), m_width(width), m_height(height), m_params(params)
  {
    GenTexture(bits);
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
    auto res = ImageUtils::LoadImage(filename.c_str(), &m_width,&m_height);
    GenTexture(res.second);
  }

  void Texture2D::GenTexture(uint width, uint height)
  {
    m_width = width;
    m_height = height;
    GenTexture({});
  }

  void Texture2D::GenTexture(const std::vector<byte>& bits)
  {
    Enable();
    if(m_params.filter != TextureFilter::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  (uint)m_params.filter));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint)m_params.filter));
    }
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)m_params.internalFormat, m_width, m_height, 0, (uint)m_params.internalFormat, GL_UNSIGNED_BYTE, bits.data()));
    Disable();
  }

  void Texture2D::SetPixels(const std::vector<byte>& pixels) const
  {
    Enable();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)m_params.internalFormat, m_width, m_height, 0, (uint)m_params.internalFormat, GL_UNSIGNED_BYTE, pixels.data()));
  }

  void Texture2D::ReloadResource()
  {
    if(texId)
      LoadTexture(filePath);
  }
}
