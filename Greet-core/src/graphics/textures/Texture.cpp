#include "Texture.h"

#include <internal/GreetGL.h>
#include <internal/OpenGLObjectHandler.h>

namespace Greet {
	
	Texture::Texture(const std::string& name, uint textureType)
		: m_name(name), m_textureType(textureType)
	{
		m_texId = OpenGLObjectHandler::CreateOpenGLObject(OpenGLType::TEXTURE);
	}

	Texture::Texture(uint texId, const std::string& name, uint textureType)
		: m_name(name), m_texId(texId), m_textureType(textureType)
	{

	}

	Texture::Texture(const Texture& texture)
	{
		operator=(texture);
	}

	Texture::~Texture()
	{
		OpenGLObjectHandler::DestroyOpenGLObject(OpenGLType::TEXTURE, m_texId);
	}

	Texture& Texture::operator=(const Texture& texture)
	{
		if (this != &texture)
		{
			OpenGLObjectHandler::CopyOpenGLObject(OpenGLType::TEXTURE, texture.m_texId);
			m_texId = texture.m_texId;
			m_textureType = texture.m_textureType;
			m_name = texture.m_name;
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
