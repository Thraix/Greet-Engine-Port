#pragma once

#include <internal/GreetTypes.h>
#include <math/Maths.h>
#include <graphics/textures/Texture2D.h>

namespace Greet{
	class Sprite
	{
	protected:
		Texture2D* m_texture;
		Vec2 m_texPos;
		Vec2 m_texSize;
	public:
		Sprite()
			: m_texture(NULL), m_texPos(Vec2(0, 0)), m_texSize(Vec2(1, 1))
		{

		}

		Sprite(Texture2D* texture)
			: m_texture(texture), m_texPos(Vec2(0, 0)), m_texSize(Vec2(1, 1))
		{
		
		}

		Sprite(Texture2D* texture, Vec2 texPos, Vec2 texSize)
			: m_texture(texture), m_texPos(texPos), m_texSize(texSize)
		{

		}
		
		virtual ~Sprite() {}

		virtual bool Update(float timeElapsed) { return false; }

		inline uint GetTextureID() const { return m_texture==NULL ? 0 : m_texture->GetTexId(); }

		inline Sprite* FromSpriteSheet(Vec2 texPos, Vec2 texSize) const { return new Sprite(m_texture,m_texPos + texPos*m_texSize, m_texSize*texSize); }
		virtual inline const Vec2& GetTexPos() const { return m_texPos; }
		inline const Vec2& GetTexSize() const { return m_texSize; }
	};
}
