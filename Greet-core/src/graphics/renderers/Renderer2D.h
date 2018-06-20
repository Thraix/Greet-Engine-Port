#pragma once

#include <vector>
#include <math/Maths.h>
#include <graphics/fonts/Font.h>
#include <logging/Log.h>
#include <graphics/renderers/Renderer.h>
#include <graphics/Sprite.h>

namespace Greet{

	class Renderable;
	class Renderable2D;
	class Renderable4Poly;
	class RenderablePoly;

	class Renderer2D : public Renderer
	{
	public:
		virtual ~Renderer2D() {}
		virtual void Begin() {}
		virtual void Flush() = 0;
		virtual void End() {}

		virtual void Submit(const Renderable2D *renderable) = 0;
		virtual void Submit(const RenderablePoly *renderable) = 0;
		virtual void Submit(const Transform& transform, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize) = 0;
		virtual void Submit(const Vec2& position, const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize) = 0;
		virtual void SubmitString(const std::string& text, const Vec2& position, Font* font, const uint& color, float scale) = 0;

		virtual void DrawRect(const Vec2& position, const Vec2& size, const uint& color) = 0;
		virtual void FillRect(const Vec2& position, const Vec2& size, const uint& color) = 0;
		virtual void FillRect(const Vec2& position, const Vec2& size, const uint& color, const Sprite* mask) = 0;
	};
}
