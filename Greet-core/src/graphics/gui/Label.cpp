#include "Label.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Label);

  Label::Label(const std::string& name, Component* parent)
    : Label{name,parent, "", "", 20}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"textColor", &text.color}},
        .fonts={{"font", &text.font}}
        });
  }

  Label::Label(const std::string& name, Component* parent, const std::string& str, const std::string& fontName, float fontSize)
    : Component{name, parent}, text{str}, hasMaxWidth{false}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"textColor", &text.color}},
        .fonts={{"font", &text.font}}
        });
    text.gravity = Text::Gravity::Center;
    text.align = Text::Align::Left;
  }

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), text{object.GetText()}, hasMaxWidth{false}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"textColor", &text.color}},
        .fonts={{"font", &text.font}}
        });
    LoadStyles(object);

    std::string grav = object.GetAttribute("gravity", "center");
    if(grav == "top")
      text.gravity = Text::Gravity::Top;
    else if(grav == "bottom")
      text.gravity = Text::Gravity::Bottom;
    else
      text.gravity = Text::Gravity::Center;

    std::string sAlign = object.GetAttribute("align", "left");
    if(sAlign == "center")
      text.align = Text::Align::Center;
    else if(sAlign == "right")
      text.align = Text::Align::Right;
    else
      text.align = Text::Align::Left;

    std::string sOverlap = object.GetAttribute("overlap", "dots");
    if(sOverlap == "ignore")
      text.overlapMode = Text::OverlapMode::Ignore;
    else if(sOverlap == "wrap")
      text.overlapMode = Text::OverlapMode::Wrap;
    else
      text.overlapMode = Text::OverlapMode::Dots;
  }

  void Label::Render(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(pos + GetTotalPadding());
    text.Render(renderer, GetContentSize());
    renderer->PopTranslation();
  }

  Label& Label::SetText(const std::string& str)
  {
    text.str = str;
    if(GetWidthType() == GUISize::Type::Wrap)
      Remeasure();
    return *this;
  }

  const std::string& Label::GetText() const
  {
    return text.str;
  }

  float Label::GetWrapWidth() const
  {
    return text.GetWrapWidth();
  }

  float Label::GetWrapHeight() const
  {
    float width = text.font.GetWidthOfText(text.str);
    float maxWidth = GetWidthType() != GUISize::Type::Wrap ? Math::Min(width, GetContentSize().w) : width;
    return text.GetWrapHeight(maxWidth);
  }

  const Color& Label::GetColor() const
  {
    return text.color;
  }

  const Font& Label::GetFont() const
  {
    return text.font;
  }

  float Label::GetFontSize() const
  {
    return text.font.GetSize();
  }

  Label& Label::SetGravity(Text::Gravity grav)
  {
    text.gravity = grav;
    return *this;
  }

  Label& Label::SetAlign(Text::Align align)
  {
    text.align = align;
    return *this;
  }

  Label& Label::SetFont(const std::string& font)
  {
    text.SetFont(font);
    return *this;
  }

  Label& Label::SetFontSize(float fontSize)
  {
    text.SetFontSize(fontSize);
    Remeasure();
    return *this;
  }

  Label& Label::SetColor(const Color& color)
  {
    text.color = color;
    return *this;
  }
}
