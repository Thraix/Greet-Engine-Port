#pragma once

#include <string>
#include <internal/GreetTypes.h>

namespace Greet {
	class Texture
	{
	protected:
		uint m_texId;
		uint m_textureType;
	public:
		Texture(uint textureType, bool generateTexture = true);
		Texture(uint texId, uint textureType);
		Texture(const Texture& texture);
		Texture& operator=(const Texture& shader);
		virtual ~Texture();
		virtual void Enable() const;
		virtual void Disable() const;

		inline uint GetTexId() const { return m_texId; }
	protected:
		inline void SetTexId(uint texId) { m_texId = texId; }
	};

}
