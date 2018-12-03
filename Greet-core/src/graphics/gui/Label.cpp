#include "Label.h"

namespace Greet {
  Label::Label()
    : Component()
  {

  }

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), str{object.GetText()}, color{0,0,0,1}, hasMaxWidth{false}
  {
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, parent->GetHeight());

    if (object.HasProperty("font"))
      font = FontManager::Get(object.GetProperty("font"),fontSize);
    else
      font = FontManager::Get("",fontSize);

    size = Vec2(CalculateWidth(), CalculateHeight());
    color = GUIUtils::GetColorFromXML(object,"color",Vec4(0,0,0,1));
  }
  void Label::Render(GUIRenderer* renderer) const
  {
    renderer->SubmitString(str, pos + Vec2(0, font->GetAscender()), font, color, false);
  }

  void Label::SetText(const std::string& text)
  {
    str = text;
    size.w = CalculateWidth();
  }

  const std::string& Label::GetText() const
  {
    return str;
  }

  float Label::CalculateWidth() const
  {
    float width = font->GetWidthOfText(str);
    return hasMaxWidth ? Math::Min(width, maxWidth) : width;
  }

  float Label::CalculateHeight() const
  {
    return font->GetSize();
  }

  const Vec4& Label::GetColor() const
  {
    return color;
  }

  const Font* Label::GetFont() const
  {
    return font;
  }
}
