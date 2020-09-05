#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {

  REGISTER_COMPONENT_DEFINITION(Button);

  Button::Button(const std::string& name, Component* parent)
    : Label{name, parent}
  {
    SetGravity(Text::Gravity::Center);
    SetAlign(Text::Align::Center);
  }

  Button::Button(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize)
    : Label{name, parent, text, fontName, fontSize}
  {
    SetGravity(Text::Gravity::Center);
    SetAlign(Text::Align::Center);
  }

  Button::Button(const XMLObject& object, Component* parent)
    : Label{object, parent}
  {
    if(!object.HasAttribute("gravity"))
    {
      SetGravity(Text::Gravity::Center);
    }
    if(!object.HasAttribute("align"))
    {
      SetAlign(Text::Align::Center);
    }
  }
}
