#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {

  REGISTER_COMPONENT_DEFINITION(Button);

  Button::Button(const std::string& name, Component* parent)
    : Label{name, parent}
  {
    SetGravity(Text::Gravity::CENTER);
    SetAlign(Text::Align::CENTER);
  }

  Button::Button(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize)
    : Label{name, parent, text, fontName, fontSize}
  {
    SetGravity(Text::Gravity::CENTER);
    SetAlign(Text::Align::CENTER);
  }

  Button::Button(const XMLObject& object, Component* parent)
    : Label{object, parent}
  {
    if(!object.HasAttribute("gravity"))
    {
      SetGravity(Text::Gravity::CENTER);
    }
    if(!object.HasAttribute("align"))
    {
      SetAlign(Text::Align::CENTER);
    }
  }
}
