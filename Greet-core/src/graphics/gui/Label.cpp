#include "Label.h"

namespace Greet {
  Label::Label()
    : Component()
  {
  
  }

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), str{object.GetText()}, color{0,0,0,1}, hasMaxWidth{false}
  {
    if (object.HasProperty("fontSize"))
      fontSize = GUIUtils::CalcSize(object.GetProperty("fontSize"), parent->GetHeight());
    Style temp; currentStyle = &temp;
  
    if (object.HasProperty("font"))
      font = FontManager::Get(object.GetProperty("font"),fontSize);

    size = Vec2(GetWidth(), GetHeight());
    if(object.HasProperty("color"))
      color = GUIUtils::GetColor(object.GetProperty("color"));
    /*
		Font* font;
		Vec4 color;
    */
  }
	void Label::Render(GUIRenderer* renderer) const

	{
		renderer->SubmitString(str, pos + Vec2(0, font->GetAscender()), font, color, false);
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
