#pragma once

#include <math/Maths.h>
#include <graphics/Sprite.h>

namespace Greet {
	class Animation : public Sprite
	{
	private:
		Vec2 m_add;

		uint m_image;
		uint m_images;
		bool m_horizontalRead;
		float m_frameTime;
		float m_frameTimer;
		Vec2 m_texPos2;
	public:

	public:
		Animation(Texture2D* texture, Vec2 texPos, Vec2 texSize, float frameTimer, uint images, bool horizontalRead);
		Animation(const Sprite& sprite, float frameTimer, uint images, bool horizontalRead);
		~Animation();

		bool Update(float elapsedTime);

		void SetTexPos(Vec2 texSize);
		
		inline const Vec2& GetTexPos() const override { return m_texPos2; }
	};
}
