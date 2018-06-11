#include "FrameBufferObject.h"

#include <graphics/Window.h>

namespace Greet {

	FrameBufferObject::FrameBufferObject(uint width, uint height)
		: m_width(width), m_height(height)
	{
		GLCall(glGenFramebuffers(1, &m_fbo));


		// Color buffer
		//attachDepthTexture();
		AttachColorTexture(GL_COLOR_ATTACHMENT0);

		//return;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER,m_fbo));
		GLCall(glGenRenderbuffers(1,&m_depthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER,m_depthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width,m_height));
		
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_depthBuffer));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	FrameBufferObject::~FrameBufferObject()
	{
		GLCall(glDeleteFramebuffers(1, &m_fbo));
		GLCall(glDeleteBuffers(1,&m_depthBuffer));
		for (auto it = m_colorTextures.begin();it != m_colorTextures.end();it++)
		{
			delete it->second;
		}
		m_colorTextures.clear();
		delete m_depthTexture;
	}

	void FrameBufferObject::AttachColorTexture(uint attachmentId)
	{
		if (attachmentId < GL_COLOR_ATTACHMENT0 || attachmentId > GL_COLOR_ATTACHMENT15)
		{
			Log::Error("Color attachment is out of range: ",attachmentId);
			return;
		}
		if (m_colorTextures.find(attachmentId) != m_colorTextures.end())
		{
			Log::Error("The given attachment is already in use: ", attachmentId);
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		uint texId;
		GLCall(glGenTextures(1, &texId));
		GLCall(glBindTexture(GL_TEXTURE_2D, texId));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, texId, 0));
		m_colorTextures.emplace(attachmentId,new Texture2D(texId,m_width,m_height,24));
		uint size = m_colorTextures.size();
		uint* colorBuffers = new uint[size];
		uint i = 0;
		for (auto it = m_colorTextures.begin();it != m_colorTextures.end();it++)
			colorBuffers[i++] = it->first;
		GLCall(glDrawBuffers(size,colorBuffers));
		delete[] colorBuffers;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
	}

	void FrameBufferObject::AttachDepthTexture()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

		uint texId;
		GLCall(glGenTextures(1, &texId));
		GLCall(glBindTexture(GL_TEXTURE_2D, texId));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		//GLCall(glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);
		m_depthTexture = new Texture2D(texId,m_width,m_height,32);
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
	}

	void FrameBufferObject::Bind()
	{
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_fbo));
		GLCall(glClearColor(0,0,0,0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glViewport(0,0,m_width,m_height));
	}

	void FrameBufferObject::Unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glViewport(0,0,Window::GetWidth(),Window::GetHeight()));
	}

	Texture2D* FrameBufferObject::GetColorTexture(uint attachmentId) const 
	{ 
		auto it = m_colorTextures.find(attachmentId);
		if (it != m_colorTextures.end())
			return it->second;
		Log::Error("There is no Texture with the given attachmentId: ",attachmentId);
		return m_colorTextures.begin()->second;
	}
}
