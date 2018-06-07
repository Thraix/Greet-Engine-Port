#include "Label.h"

namespace Greet {

	Label::Label(const std::string& str, Font* font, const Vec4& color)
		: str(str), font(font), color(color)
	{
		hasMaxWidth = false;
	}

	void Label::Render(GUIRenderer* renderer, const Vec2& position) const
	{
		//renderer->SubmitString(str, position + Vec2(0, font->GetAscender()), font, color, true);
	}


	float Label::GetWidth() const
	{
		float width = font->GetWidthOfText(str);
		return hasMaxWidth ? Math::Min(width, maxWidth) : width;
	}

	float Label::GetHeight() const
	{
		return font->GetSize();
	}
}
