#include "Texture.h"

#include <internal/GreetGL.h>
#include <internal/OpenGLObjectHandler.h>

namespace Greet {
	
	Texture::Texture(const std::string& name, uint textureType)
		: m_name(name), m_textureType(textureType)
	{
		m_texId = OpenGLObjectHandler::CreateOpenGLObject(OpenGLType::TEXTURE);
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
	}

	Texture::Texture(uint texId, const std::string& name, uint textureType)
		: m_name(name), m_texId(texId), m_textureType(textureType)
	{
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
	}

  Texture::Texture(uint textureType)
    :m_name("NULL"), m_texId(0), m_textureType(textureType)
  {
   ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
  }

	Texture::Texture(const Texture& texture)
	{
		operator=(texture);
	}

	Texture::~Texture()
	{
    if(m_texId != 0)
      OpenGLObjectHandler::DestroyOpenGLObject(OpenGLType::TEXTURE, m_texId);
	}

	Texture& Texture::operator=(const Texture& texture)
	{
		if (this != &texture)
		{
      if(texture.m_texId != 0)
        OpenGLObjectHandler::CopyOpenGLObject(OpenGLType::TEXTURE, texture.m_texId);
			this->m_texId = texture.m_texId;
			this->m_textureType = texture.m_textureType;
			this->m_name = texture.m_name;
		}
		return *this;
	}

	void Texture::Enable() const
	{
    GLCall(glBindTexture(m_textureType, m_texId));
	}

	void Texture::Disable() const
	{
		GLCall(glBindTexture(m_textureType, 0));
	}

}
