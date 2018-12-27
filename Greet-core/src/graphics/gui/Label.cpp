#include "Label.h"

namespace Greet {

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), str{object.GetText()}, color{0,0,0,1}, hasMaxWidth{false}
  {
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, parent->GetHeight());
    font = FontManager::Get(GUIUtils::GetStringFromXML(object,"font",""),fontSize);

    std::string grav = object.GetProperty("gravity", "center");
    if(grav == "top")
      gravity = Gravity::TOP;
    else if(grav == "bottom")
      gravity = Gravity::BOTTOM;
    else// if(grav == "center") or invalid
      gravity = Gravity::CENTER;

    color = GUIUtils::GetColorFromXML(object,"color",Vec4(0,0,0,1));
  }

  void Label::Render(GUIRenderer* renderer) const
  {
    if(gravity == Gravity::TOP)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, font->GetBaselineOffset()), font, color, false);
    else if(gravity == Gravity::CENTER)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, GetContentSize().h + font->GetBaselineOffset())/2, font, color, false);
    else// if(gravity == Gravity::BOTTOM)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, GetContentSize().h), font, color, false);
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
