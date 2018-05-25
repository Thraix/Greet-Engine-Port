#pragma once

#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/Window.h>
#include <map>

namespace Greet {

	class FrameBufferObject
	{
	private:
		uint m_fbo;
		uint m_width;
		uint m_height;
		std::map<uint, Texture2D*> m_colorTextures;
		Texture2D* m_depthTexture;
		uint m_depthBuffer;
	public:
		FrameBufferObject(uint width, uint height);
		virtual ~FrameBufferObject();
		void Bind();
		void Unbind();
		void AttachColorTexture(uint attachementId);
		void AttachDepthTexture();
		Texture2D* GetColorTexture(uint attachmentId) const;
		inline Texture2D* GetDepthTexture() const { return m_depthTexture; }
	private:
	};
}