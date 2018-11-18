#pragma once

#include "Component.h"
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {
	class Label : public Component 
	{
	private:
		bool hasMaxWidth;
	public:
		float maxWidth;
		std::string str;
    float fontSize;
		Font* font;
		Vec4 color;

	public:
    Label();
    Label(const XMLObject& object, Component* parent);
		void Render(GUIRenderer* renderer) const override;

		float GetWidth() const override;
		float GetHeight() const override;
	};
}
