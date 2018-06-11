#pragma once

#include <graphics/textures/Texture.h>
#include <utils/ImageUtils.h>

namespace Greet {
	class Texture2D : public Texture
	{
		friend class FrameBufferObject;
	protected:
		uint m_width;
		uint m_height;
    uint m_bpp;
	public:
    Texture2D(uint width, uint height, uint bpp);
		Texture2D(const std::string& filename);
		Texture2D(BYTE* bits, uint width, uint height, uint bpp);
		Texture2D(uint texID, uint width, uint height, uint bpp);
    Texture2D();
    void SetPixels(const void* pixels) const;
		virtual ~Texture2D();
		inline uint GetWidth() const { return m_width; }
		inline uint GetHeight() const { return m_height; }
	protected:
		void LoadTexture(const std::string& filename);
		void GenTexture(uint width, uint height, uint bpp);
		void GenTexture(BYTE* bits, uint bpp);

	};
}
