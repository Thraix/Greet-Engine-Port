#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Label.h>

namespace Greet {
  class Button : public Label
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Button);
    public:
      Button(const std::string& name, Component* parent);
      Button(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize);
      Button(const XMLObject& object, Component* parent);
  };
}
