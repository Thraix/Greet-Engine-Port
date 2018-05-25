#pragma once

#include <logging/Log.h>
#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>
#include <FreeImage.h>
#include <graphics/textures/Texture.h>

namespace Greet {
	class Texture2D : public Texture
	{
		friend class FrameBufferObject;
	protected:
		uint m_width;
		uint m_height;
	public:
		Texture2D(const std::string& filename, const std::string& name);
		Texture2D(BYTE* bits, uint width, uint height, uint bpp, const std::string& name);
		Texture2D(uint texID, uint width, uint height, const std::string& name);
		virtual ~Texture2D();
		inline uint GetWidth() const { return m_width; }
		inline uint GetHeight() const { return m_height; }
	protected:
		void LoadTexture(const std::string& filename);
		void GenTexture(uint width, uint height, uint bpp);
		void GenTexture(BYTE* bits, uint bpp);

	};
}
