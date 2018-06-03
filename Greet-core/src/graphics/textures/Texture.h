#pragma once

#include <string>
#include <internal/GreetTypes.h>

namespace Greet {
	class Texture
	{
	protected:
		uint m_texId;
		uint m_textureType;
		std::string m_name;
	public:
		Texture(const std::string& name, uint textureType);
		Texture(uint texId, const std::string& name, uint textureType);
		Texture(const Texture& texture);
		Texture& operator=(const Texture& shader);
		virtual ~Texture();
		virtual void Enable() const;
		virtual void Disable() const;

		inline uint GetTexId() const { return m_texId; }
		inline const std::string& GetName() const { return m_name; }
	protected:
		inline void SetTexId(uint texId) { m_texId = texId; }
	};

}
