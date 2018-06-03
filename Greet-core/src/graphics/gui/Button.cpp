#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {
	Button::Button(const Vec2& size, const std::string& text)
		: size(size)
	{
		label = new Label(text, FontManager::Get("roboto", (int)(size.y*0.8)), ColorUtils::GetMaterialColorAsHSV(0.5, 2));
		AddContent(label);
		float margin = (size.x - label->GetWidth()) / 2;
		SetMargins(margin, margin, 0, 0);
	}

	Button::Button(const Vec2& size, Content* content)
		: size(size)
	{
		AddContent(content);
	}

	Button::~Button()
	{
		if (label != NULL)
			delete label;
	}

	void Button::Render(GUIRenderer* renderer, const Vec2& position) const
	{
		renderer->SubmitRect(position, GetSize(), ColorUtils::GetMaterialColorAsHSV(0.5, 6), true);
		Content::Render(renderer, position);
	}


	float Button::GetWidth() const
	{
		return size.x;
	}

	float Button::GetHeight() const
	{
		return size.y;
	}

	bool Button::IsFocusable() const
	{
		return true;
	}
}