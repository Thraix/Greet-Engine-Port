#include "Texture2D.h"

#include <internal/GreetGL.h>
#include <internal/OpenGLObjectHandler.h>

namespace Greet {

  Texture2D::Texture2D(uint width, uint height, uint bpp)
    : Texture(GL_TEXTURE_2D)
  {
    GenTexture(width,height,bpp);
  }
  
  Texture2D::Texture2D(const std::string& filename)
		:Texture(GL_TEXTURE_2D)
	{
		LoadTexture(filename);
	}

	Texture2D::Texture2D(BYTE* bits, uint width, uint height, uint bpp)
		: Texture(GL_TEXTURE_2D), m_width(width), m_height(height)
	{
		GenTexture(bits, bpp);
		delete[] bits;
	}

  Texture2D::Texture2D()
    : Texture(GL_TEXTURE_2D,false)
  {
    
  }

	Texture2D::Texture2D(uint texId, uint width, uint height, uint bpp)
		:Texture(texId, (uint)GL_TEXTURE_2D), m_width(width), m_height(height), m_bpp(bpp)
	{

	}

	Texture2D::~Texture2D()
	{
	
	}

	void Texture2D::LoadTexture(const std::string& filename)
	{
		uint bpp = 0;
		BYTE* bits = ImageUtils::loadImage(filename.c_str(),&m_width,&m_height,&bpp);
		GenTexture(bits, bpp);
		delete[] bits;
	}

	void Texture2D::GenTexture(uint width, uint height, uint bpp)
	{
		m_width = width;
		m_height = height;
    m_bpp = bpp;
		GenTexture(NULL,bpp);
	}

	void Texture2D::GenTexture(BYTE* bits, uint bpp)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_texId));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, bpp == 32 ? GL_RGBA : GL_RGB, m_width, m_height, 0, bpp == 32 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, bits));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

  void Texture2D::SetPixels(const void* pixels) const
  {
    Enable();
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_bpp == 32 ? GL_BGRA : GL_BGR,GL_UNSIGNED_BYTE, pixels));
  }
}
