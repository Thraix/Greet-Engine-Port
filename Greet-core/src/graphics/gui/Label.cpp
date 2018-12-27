#include "Label.h"

namespace Greet {

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), str{object.GetText()}, color{0,0,0,1}, hasMaxWidth{false}
  {
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, parent->GetHeight());
    font = FontManager::Get(GUIUtils::GetStringFromXML(object,"font",""),fontSize);

    color = GUIUtils::GetColorFromXML(object,"color",Vec4(0,0,0,1));
  }

  void Label::Render(GUIRenderer* renderer) const
  {
    // Top of the text will be at 0
    renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, font->GetBaselineOffset()), font, color, false);
  }

  void Label::SetText(const std::string& text)
  {
    str = text;
    Remeasure();
  }

  const std::string& Label::GetText() const
  {
    return str;
  }

  Vec2 Label::GetWrapSize() const
  {
    float width = font->GetWidthOfText(str);
    return Vec2(hasMaxWidth ? Math::Min(width, maxWidth) : width, font->GetSize());
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
