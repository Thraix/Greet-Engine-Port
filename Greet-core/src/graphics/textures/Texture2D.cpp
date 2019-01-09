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
    uint bpp = 0;
    BYTE* bits = ImageUtils::loadImage(filename.c_str(),&m_width,&m_height,&bpp);
    switch(bpp)
    {
      case 8:
        if(m_params.internalFormat != TextureInternalFormat::RED && m_params.internalFormat != TextureInternalFormat::DEPTH_COMPONENT && m_params.internalFormat != TextureInternalFormat::DEPTH_STENCIL)
        {
          Log::Warning("Image bpp is 8 but TextureInternalFormat is not RED, DEPTH_COMPONENT or DEPTH_STENCIL");
        }
        break;
      case 24:
        if(m_params.internalFormat != TextureInternalFormat::RGB)
        {
          Log::Warning("Image bpp is 24 but TextureInternalFormat is not RGB (filename=",filename,")");
        }
        break;
      case 32:
        if(m_params.internalFormat != TextureInternalFormat::RGBA)
        {
          Log::Warning("Image bpp is 32 but TextureInternalFormat is not RGBA (filename=",filename,")");
        }
        break;
      default:
        Log::Warning("Image bpp is invalid (bpp=",bpp,")");
    }
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
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)m_params.internalFormat, m_width, m_height, 0, GetFormat(), GL_UNSIGNED_BYTE, bits));
    Disable();
  }

  void Texture2D::SetPixels(const void* pixels) const
  {
    Enable();
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GetFormat(),GL_UNSIGNED_BYTE, pixels));
  }

  uint Texture2D::GetFormat() const
  {
    bool inverted = m_params.format == TextureFormat::INVERTED? true : false;
    if(m_params.internalFormat == TextureInternalFormat::RGBA)
    {
      return inverted ? GL_BGRA : GL_RGBA;
    }
    else if(m_params.internalFormat == TextureInternalFormat::RGB)
    {
      return inverted ? GL_BGR : GL_RGB;
    }
    else if(m_params.internalFormat == TextureInternalFormat::RED)
    {
      return GL_RED;
    }
    else if(m_params.internalFormat == TextureInternalFormat::DEPTH_COMPONENT)
    {
      return GL_DEPTH_COMPONENT;
    }
    else //if(m_params.internalFormat == TextureInternalFormat::DEPTH_STENCIL)
    {
      return GL_DEPTH_STENCIL;
    }
  }
}
