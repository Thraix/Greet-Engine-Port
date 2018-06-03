#pragma once

#include "Content.h"
#include <graphics/fonts/Font.h>

namespace Greet {
	class Label : public Content
	{
	private:
		bool hasMaxWidth;
	public:
		float maxWidth;
		std::string str;
		Font* font;
		Vec4 color;

	public:
		Label(const std::string& str, Font* font, const Vec4& color);
		void Render(GUIRenderer* renderer, const Vec2& position) const override;

		float GetWidth() const override;
		float GetHeight() const override;
	};
}