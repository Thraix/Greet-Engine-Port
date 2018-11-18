#pragma once

#include "Component.h"
#include <graphics/fonts/Font.h>

namespace Greet {
	class Label : public Component 
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
		void Render(GUIRenderer* renderer) const override;

		float GetWidth() const override;
		float GetHeight() const override;
	};
}
