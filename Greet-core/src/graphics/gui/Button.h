#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Label.h>

namespace Greet {
  class Button : public Component 
  {
    protected:
      float fontSize;
      Vec4 textColor;
      Label label;
    public:
      Button(const XMLObject& object, Component* parent);
      virtual ~Button();
      void Render(GUIRenderer* renderer) const override;

      virtual void SetPosition(const Vec2& pos) override;


      void OnFocused() override;
      void OnUnfocused() override;
      void MouseEntered() override;
      void MouseExited() override;
  };
}
